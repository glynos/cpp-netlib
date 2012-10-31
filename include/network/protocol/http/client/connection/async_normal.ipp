// Copyright 2011 Dean Michael Berris <dberris@google.com>.
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_ASYNC_NORMAL_IPP_20111123
#define NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_ASYNC_NORMAL_IPP_20111123

#include <boost/asio/placeholders.hpp>
#include <network/protocol/http/client/connection/async_normal.hpp>
#include <network/protocol/http/request.hpp>
#include <network/protocol/http/response.hpp>
#include <network/protocol/http/client/connection/connection_delegate.hpp>
#include <network/protocol/http/client/connection/resolver_delegate.hpp>
#include <network/protocol/http/algorithms/linearize.hpp>
#include <network/protocol/http/impl/access.hpp>
#include <boost/asio/strand.hpp>
#include <network/detail/debug.hpp>
#ifdef NETWORK_ENABLE_HTTPS
#include <boost/asio/ssl/error.hpp>
#endif

namespace network { namespace http {

namespace placeholders = boost::asio::placeholders;

struct http_async_connection_pimpl : boost::enable_shared_from_this<http_async_connection_pimpl>
{
  typedef http_async_connection::callback_type body_callback_function_type;
  typedef resolver_delegate::resolver_iterator resolver_iterator;
  typedef resolver_delegate::iterator_pair resolver_iterator_pair;
  typedef http_async_connection_pimpl this_type;

  http_async_connection_pimpl(
    boost::shared_ptr<resolver_delegate> resolver_delegate,
    boost::shared_ptr<connection_delegate> connection_delegate,
    boost::asio::io_service & io_service,
    bool follow_redirect)
  :
        follow_redirect_(follow_redirect),
        request_strand_(io_service),
        resolver_delegate_(resolver_delegate),
        connection_delegate_(connection_delegate) {
    NETWORK_MESSAGE("http_async_connection_pimpl::http_async_connection_pimpl(...)");
  }

  // This is the main entry point for the connection/request pipeline. We're
  // overriding async_connection_base<...>::start(...) here which is called
  // by the client.
  response start(request const & request,
                 std::string const & method,
                 bool get_body,
                 body_callback_function_type callback,
                 request_options const &options) {
    NETWORK_MESSAGE("http_async_connection_pimpl::start(...)");
    response response_;
    this->init_response(response_);
    // Use HTTP/1.1 -- at some point we might want to implement a different
    // connection type just for HTTP/1.0.
    // TODO: Implement a different connection type and factory for HTTP/1.0.
    linearize(request, method, 1, 1,
      std::ostreambuf_iterator<char>(&command_streambuf));
    this->method = method;
    NETWORK_MESSAGE("method: " << this->method);
    boost::uint16_t port_ = port(request);
    NETWORK_MESSAGE("port: " << port_);
    this->host_ = host(request);
    resolver_delegate_->resolve(
        this->host_,
        port_,
        request_strand_.wrap(
            boost::bind(
                &this_type::handle_resolved,
                this_type::shared_from_this(),
                port_,
                get_body,
                callback,
                _1,
                _2)));
    return response_;
  }

  http_async_connection_pimpl * clone() {
    NETWORK_MESSAGE("http_async_connection_pimpl::clone()");
    return new (std::nothrow) http_async_connection_pimpl(
        this->resolver_delegate_,
        this->connection_delegate_,
        request_strand_.get_io_service(),
        follow_redirect_);
  }

  void reset() {
    // FIXME Perform the actual re-set of the internal state and pending stuff.
  }

 private:

  http_async_connection_pimpl(http_async_connection_pimpl const &); // = delete

  void init_response(response &r) {
    NETWORK_MESSAGE("http_async_connection_pimpl::init_response(...)");
    impl::setter_access accessor;
    accessor.set_source_promise(r, this->source_promise);
    accessor.set_destination_promise(r, this->destination_promise);
    accessor.set_headers_promise(r, this->headers_promise);
    accessor.set_body_promise(r, this->body_promise);
    accessor.set_version_promise(r, this->version_promise);
    accessor.set_status_promise(r, this->status_promise);
    accessor.set_status_message_promise(r, this->status_message_promise);
    NETWORK_MESSAGE("futures and promises lined up.");
  }

  void set_errors(boost::system::error_code const & ec) {
    NETWORK_MESSAGE("http_async_connection_pimpl::set_errors(...)");
    NETWORK_MESSAGE("error: " << ec);
    boost::system::system_error error(ec);
    this->version_promise.set_exception(boost::copy_exception(error));
    this->status_promise.set_exception(boost::copy_exception(error));
    this->status_message_promise.set_exception(boost::copy_exception(error));
    this->headers_promise.set_exception(boost::copy_exception(error));
    this->source_promise.set_exception(boost::copy_exception(error));
    this->destination_promise.set_exception(boost::copy_exception(error));
    this->body_promise.set_exception(boost::copy_exception(error));
    NETWORK_MESSAGE("promise+future exceptions set.");
  }

  void handle_resolved(boost::uint16_t port,
                       bool get_body,
                       body_callback_function_type callback,
                       boost::system::error_code const & ec,
                       resolver_iterator_pair endpoint_range) {
    NETWORK_MESSAGE("http_async_connection_pimpl::handle_resolved(...)");
    if (!ec && !boost::empty(endpoint_range)) {
      // Here we deal with the case that there was no error encountered.
      NETWORK_MESSAGE("resolved endpoint successfully");
      resolver_iterator iter = boost::begin(endpoint_range);
      NETWORK_MESSAGE("trying connection to: "
                            << iter->endpoint().address() << ":" << port);
      boost::asio::ip::tcp::endpoint endpoint(iter->endpoint().address(), port);
      connection_delegate_->connect(
          endpoint,
          this->host_,
          request_strand_.wrap(
              boost::bind(
                  &this_type::handle_connected,
                  this_type::shared_from_this(),
                  port,
                  get_body,
                  callback,
                  std::make_pair(++iter,
                                 resolver_iterator()),
                  placeholders::error)));
    } else {
      NETWORK_MESSAGE("error encountered while resolving.");
      set_errors(ec ? ec : boost::asio::error::host_not_found);
    }
  }

  void handle_connected(boost::uint16_t port,
                        bool get_body,
                        body_callback_function_type callback,
                        resolver_iterator_pair endpoint_range,
                        boost::system::error_code const & ec) {
    NETWORK_MESSAGE("http_async_connection_pimpl::handle_connected(...)");
    if (!ec) {
      NETWORK_MESSAGE("connected successfully");
      BOOST_ASSERT(connection_delegate_.get() != 0);
      NETWORK_MESSAGE("scheduling write...");
      connection_delegate_->write(command_streambuf,
                       request_strand_.wrap(
                           boost::bind(
                               &this_type::handle_sent_request,
                               this_type::shared_from_this(),
                               get_body,
                               callback,
                               placeholders::error,
                               placeholders::bytes_transferred)));
    } else {
      NETWORK_MESSAGE("connection unsuccessful");
      if (!boost::empty(endpoint_range)) {
        resolver_iterator iter = boost::begin(endpoint_range);
        NETWORK_MESSAGE("trying: " << iter->endpoint().address() << ":" << port);
        boost::asio::ip::tcp::endpoint endpoint(iter->endpoint().address(), port);
        connection_delegate_->connect(endpoint,
                           this->host_,
                           request_strand_.wrap(
                               boost::bind(
                                   &this_type::handle_connected,
                                   this_type::shared_from_this(),
                                   port,
                                   get_body,
                                   callback,
                                   std::make_pair(++iter,
                                                  resolver_iterator()),
                                   placeholders::error)));
      } else {
        set_errors(ec ? ec : boost::asio::error::host_not_found);
      }
    }
  }

  enum state_t {
    version, status, status_message, headers, body
  };

  void handle_sent_request(bool get_body,
                           body_callback_function_type callback,
                           boost::system::error_code const & ec,
                           std::size_t bytes_transferred) {
    NETWORK_MESSAGE("http_async_connection_pimpl::handle_sent_request(...)");
    if (!ec) {
      NETWORK_MESSAGE("request sent successfuly; scheduling partial read...");
      connection_delegate_->read_some(
          boost::asio::mutable_buffers_1(this->part.c_array(),
                                         this->part.size()),
          request_strand_.wrap(
              boost::bind(&this_type::handle_received_data,
                          this_type::shared_from_this(),
                          version, get_body, callback,
                          placeholders::error,
                          placeholders::bytes_transferred)));
    } else {
      NETWORK_MESSAGE("request sent unsuccessfully; setting errors");
      set_errors(ec);
    }
  }

  void handle_received_data(state_t state, bool get_body, body_callback_function_type callback, boost::system::error_code const & ec, std::size_t bytes_transferred) {
    NETWORK_MESSAGE("http_async_connection_pimpl::handle_received_data(...)");
    // Okay, there's some weirdness with Boost.Asio's handling of SSL errors
    // so we need to do some acrobatics to make sure that we're handling the
    // short-read errors correctly. This is such a PITA that we have to deal
    // with this here.
    static long short_read_error = 335544539;
    bool is_short_read_error =
#ifdef NETWORK_ENABLE_HTTPS
        ec.category() == boost::asio::error::ssl_category &&
        ec.value() == short_read_error
#else
        false
#endif
        ;
    if (!ec || ec == boost::asio::error::eof || is_short_read_error) {
      NETWORK_MESSAGE("processing data chunk, no error encountered so far...");
      boost::logic::tribool parsed_ok;
      size_t remainder;
      switch(state) {
        case version:
          NETWORK_MESSAGE("parsing version...");
          parsed_ok =
              this->parse_version(request_strand_.wrap(
                                      boost::bind(
                                          &this_type::handle_received_data,
                                          this_type::shared_from_this(),
                                          version, get_body, callback,
                                          placeholders::error,
                                          placeholders::bytes_transferred)),
                                  bytes_transferred);
          if (!parsed_ok || indeterminate(parsed_ok)) return;
        case status:
          NETWORK_MESSAGE("parsing status...");
          parsed_ok =
              this->parse_status(request_strand_.wrap(
                                     boost::bind(
                                         &this_type::handle_received_data,
                                         this_type::shared_from_this(),
                                         status, get_body, callback,
                                         placeholders::error,
                                         placeholders::bytes_transferred)),
                                 bytes_transferred);
          if (!parsed_ok || indeterminate(parsed_ok)) return;
        case status_message:
          NETWORK_MESSAGE("parsing status message...");
          parsed_ok =
            this->parse_status_message(
              request_strand_.wrap(
                boost::bind(
                  &this_type::handle_received_data,
                  this_type::shared_from_this(),
                  status_message, get_body, callback,
                  placeholders::error, placeholders::bytes_transferred
                  )
                ),
              bytes_transferred
              );
          if (!parsed_ok || indeterminate(parsed_ok)) return;
        case headers:
          NETWORK_MESSAGE("parsing headers...");
          // In the following, remainder is the number of bytes that remain
          // in the buffer. We need this in the body processing to make sure
          // that the data remaining in the buffer is dealt with before
          // another call to get more data for the body is scheduled.
          boost::fusion::tie(parsed_ok, remainder) =
            this->parse_headers(
              request_strand_.wrap(
                boost::bind(
                  &this_type::handle_received_data,
                  this_type::shared_from_this(),
                  headers, get_body, callback,
                  placeholders::error, placeholders::bytes_transferred
                  )
                ),
              bytes_transferred
              );

          if (!parsed_ok || indeterminate(parsed_ok)) return;

          if (!get_body) {
            NETWORK_MESSAGE("not getting body...");
            // We short-circuit here because the user does not
            // want to get the body (in the case of a HEAD
            // request).
            this->body_promise.set_value("");
            this->destination_promise.set_value("");
            this->source_promise.set_value("");
            this->part.assign('\0');
            this->response_parser_.reset();
            NETWORK_MESSAGE("processing done.");
            return;
          }

          if (callback) {
            NETWORK_MESSAGE("callback provided, processing body...");
            // Here we deal with the spill-over data from the
            // headers processing. This means the headers data
            // has already been parsed appropriately and we're
            // looking to treat everything that remains in the
            // buffer.
            buffer_type::const_iterator begin = this->part_begin;
            buffer_type::const_iterator end = begin;
            std::advance(end, std::min(bytes_transferred, remainder));

            // We're setting the body promise here to an empty string because
            // this can be used as a signaling mechanism for the user to
            // determine that the body is now ready for processing, even
            // though the callback is already provided.
            this->body_promise.set_value("");

            // The invocation of the callback is synchronous to allow us to
            // wait before scheduling another read.
            callback(boost::make_iterator_range(begin, end), ec);

            connection_delegate_->read_some(
                boost::asio::mutable_buffers_1(this->part.c_array(),
                                               this->part.size()),
                request_strand_.wrap(
                    boost::bind(&this_type::handle_received_data,
                                this_type::shared_from_this(),
                                body,
                                get_body,
                                callback,
                                placeholders::error,
                                placeholders::bytes_transferred)));
          } else {
            NETWORK_MESSAGE("no callback provided, appending to body...");
            // Here we handle the body data ourself and append to an
            // ever-growing string buffer.
            this->parse_body(
              request_strand_.wrap(
                boost::bind(
                  &this_type::handle_received_data,
                  this_type::shared_from_this(),
                  body, get_body, callback,
                  placeholders::error, placeholders::bytes_transferred
                  )
                ),
              remainder);
          }
          return;
        case body:
          NETWORK_MESSAGE("parsing body...");
          if (ec == boost::asio::error::eof || is_short_read_error) {
            NETWORK_MESSAGE("end of the line.");
            // Here we're handling the case when the connection has been
            // closed from the server side, or at least that the end of file
            // has been reached while reading the socket. This signals the end
            // of the body processing chain.
            if (callback) {
              NETWORK_MESSAGE("callback provided, invoking callback...");
              buffer_type::const_iterator begin =
                  this->part.begin(),
                  end = begin;
              std::advance(end, bytes_transferred);

              // We call the callback function synchronously passing the error
              // condition (in this case, end of file) so that it can handle
              // it appropriately.
              callback(boost::make_iterator_range(begin, end), ec);
            } else {
              NETWORK_MESSAGE("no callback provided, appending to body...");
              std::string body_string;
              std::swap(body_string, this->partial_parsed);
              body_string.append(
                this->part.begin()
                , bytes_transferred
                );
              this->body_promise.set_value(body_string);
            }
            // TODO set the destination value somewhere!
            this->destination_promise.set_value("");
            this->source_promise.set_value("");
            this->part.assign('\0');
            this->response_parser_.reset();
          } else {
            NETWORK_MESSAGE("connection still active...");
            // This means the connection has not been closed yet and we want to get more
            // data.
            if (callback) {
              NETWORK_MESSAGE("callback provided, invoking callback...");
              // Here we have a body_handler callback. Let's invoke the
              // callback from here and make sure we're getting more data
              // right after.
              buffer_type::const_iterator begin = this->part.begin();
              buffer_type::const_iterator end = begin;
              std::advance(end, bytes_transferred);
              callback(boost::make_iterator_range(begin, end), ec);
              connection_delegate_->read_some(
                  boost::asio::mutable_buffers_1(
                      this->part.c_array(),
                      this->part.size()),
                  request_strand_.wrap(
                      boost::bind(
                          &this_type::handle_received_data,
                          this_type::shared_from_this(),
                          body,
                          get_body,
                          callback,
                          placeholders::error,
                          placeholders::bytes_transferred)));
            } else {
              NETWORK_MESSAGE("no callback provided, appending to body...");
              bool get_more = true;
              if (content_length_) {
                buffer_type::const_iterator begin = this->part.begin();
                buffer_type::const_iterator end = begin;
                std::advance(end, bytes_transferred);
                get_more = (end - begin) < *content_length_;
                NETWORK_MESSAGE("content_length = " << * content_length_ 
                  << ", bytes read = " << (end - begin) << ", read more = " << get_more);
              }
              // Here we don't have a body callback. Let's
              // make sure that we deal with the remainder
              // from the headers part in case we do have data
              // that's still in the buffer.
              if (get_more) {
                this->parse_body(request_strand_.wrap(
                                     boost::bind(
                                         &this_type::handle_received_data,
                                         this_type::shared_from_this(),
                                         body,
                                         get_body,
                                         callback,
                                         placeholders::error,
                                         placeholders::bytes_transferred)),
                                 bytes_transferred);
              } else {
                std::string body_string;
                std::swap(body_string, this->partial_parsed);
                body_string.append(
                  this->part.begin()
                  , bytes_transferred
                  );
                this->body_promise.set_value(body_string);                
                // TODO set the destination value somewhere!
                this->destination_promise.set_value("");
                this->source_promise.set_value("");
                this->part.assign('\0');
                this->response_parser_.reset();
              }
            }
          }
          return;
        default:
          BOOST_ASSERT(false && "Bug, report this to the developers!");
      }
    } else {
      boost::system::system_error error(ec);
      NETWORK_MESSAGE("error encountered: " << error.what() << " (" << ec << ")");
      this->source_promise.set_exception(boost::copy_exception(error));
      this->destination_promise.set_exception(boost::copy_exception(error));
      switch (state) {
        case version:
          this->version_promise.set_exception(boost::copy_exception(error));
        case status:
          this->status_promise.set_exception(boost::copy_exception(error));
        case status_message:
          this->status_message_promise.set_exception(boost::copy_exception(error));
        case headers:
          this->headers_promise.set_exception(boost::copy_exception(error));
        case body:
          this->body_promise.set_exception(boost::copy_exception(error));
          break;
        default:
          BOOST_ASSERT(false && "Bug, report this to the developers!");
      }
    }
  }

#ifdef NETWORK_DEBUG
  struct debug_escaper {
    std::string & string;
    explicit debug_escaper(std::string & string_)
      : string(string_) {}
    debug_escaper(debug_escaper const & other)
      : string(other.string) {}
    void operator()( std::string::value_type input) {
      if (!boost::algorithm::is_print()(input)) {
        std::ostringstream escaped_stream;
        if (input == '\r') {
          string.append("\\r");
        } else if (input == '\n') {
          string.append("\\n");
        } else {
          escaped_stream << "\\x" << static_cast<int>(input);
          string.append(escaped_stream.str());
        }
      } else {
        string.push_back(input);
      }
    }
  };
#endif
    
  struct to_http_headers {
    template <class U>
    std::string const operator() (U const & pair) const {
      std::ostringstream header_line;
      header_line << pair.first
        << constants::colon()
        << constants::space()
        << pair.second
        << constants::crlf();
      return header_line.str();
    }
  };

  boost::logic::tribool parse_version(
      boost::function<void(boost::system::error_code, size_t)> callback,
      size_t bytes) {
    boost::logic::tribool parsed_ok;
    part_begin = part.begin();
    buffer_type::const_iterator part_end = part.begin();
    std::advance(part_end, bytes);
    boost::iterator_range<buffer_type::const_iterator>
      result_range,
      input_range = boost::make_iterator_range(part_begin, part_end);
    boost::fusion::tie(parsed_ok, result_range) = response_parser_.parse_until(
      response_parser::http_version_done,
      input_range);
    if (parsed_ok == true) {
      std::string version;
      std::swap(version, partial_parsed);
      version.append(boost::begin(result_range),
               boost::end(result_range));
      boost::algorithm::trim(version);
      version_promise.set_value(version);
      part_begin = boost::end(result_range);
    } else if (parsed_ok == false) {
#ifdef NETWORK_DEBUG
      std::string escaped;
      debug_escaper escaper(escaped);
      std::for_each(part_begin, part_end, escaper);
      NETWORK_MESSAGE("[parser:"
                  << response_parser_.state()
                  << "] buffer contents: \""
                  << escaped
                  << "\"");
#endif
      std::runtime_error error("Invalid Version Part.");
      version_promise.set_exception(boost::copy_exception(error));
      status_promise.set_exception(boost::copy_exception(error));
      status_message_promise.set_exception(
        boost::copy_exception(error));
      headers_promise.set_exception(boost::copy_exception(error));
      source_promise.set_exception(boost::copy_exception(error));
      destination_promise.set_exception(boost::copy_exception(error));
      body_promise.set_exception(boost::copy_exception(error));
    } else {
      partial_parsed.append(
        boost::begin(result_range),
        boost::end(result_range)
        );
      part_begin = part.begin();
      connection_delegate_->read_some(
        boost::asio::mutable_buffers_1(part.c_array(), part.size()),
        callback
        );
    }
    return parsed_ok;
  }
    
  boost::logic::tribool parse_status(
      boost::function<void(boost::system::error_code, size_t)> callback,
      size_t bytes) {
    boost::logic::tribool parsed_ok;
     buffer_type::const_iterator part_end = part.begin();
    std::advance(part_end, bytes);
     boost::iterator_range< buffer_type::const_iterator>
      result_range,
      input_range = boost::make_iterator_range(part_begin, part_end);
    boost::fusion::tie(parsed_ok, result_range) = response_parser_.parse_until(
      response_parser::http_status_done,
      input_range);
    if (parsed_ok == true) {
      std::string status;
      std::swap(status, partial_parsed);
      status.append(boost::begin(result_range),
              boost::end(result_range));
      boost::trim(status);
      boost::uint16_t status_int =
        boost::lexical_cast<boost::uint16_t>(status);
      status_promise.set_value(status_int);
      part_begin = boost::end(result_range);
    } else if (parsed_ok == false) {
#ifdef NETWORK_DEBUG
      std::string escaped;
      debug_escaper escaper(escaped);
      std::for_each(part_begin, part_end, escaper);
      NETWORK_MESSAGE("[parser:"
                  << response_parser_.state()
                  << "] buffer contents: \""
                  << escaped
                  << "\"");
#endif
      std::runtime_error error("Invalid status part.");
      status_promise.set_exception(boost::copy_exception(error));
      status_message_promise.set_exception(
        boost::copy_exception(error));
      headers_promise.set_exception(boost::copy_exception(error));
      source_promise.set_exception(boost::copy_exception(error));
      destination_promise.set_exception(boost::copy_exception(error));
      body_promise.set_exception(boost::copy_exception(error));
    } else {
      partial_parsed.append(
        boost::begin(result_range),
        boost::end(result_range)
        );
      part_begin = part.begin();
      connection_delegate_->read_some(
        boost::asio::mutable_buffers_1(part.c_array(), part.size()),
        callback
        );
    }
    return parsed_ok;
  }

  boost::logic::tribool parse_status_message(
      boost::function<void(boost::system::error_code, size_t)> callback,
      size_t bytes) {
    boost::logic::tribool parsed_ok;
     buffer_type::const_iterator part_end = part.begin();
    std::advance(part_end, bytes);
     boost::iterator_range< buffer_type::const_iterator>
      result_range,
      input_range = boost::make_iterator_range(part_begin, part_end);
    boost::fusion::tie(parsed_ok, result_range) = response_parser_.parse_until(
      response_parser::http_status_message_done,
      input_range);
    if (parsed_ok == true) {
      std::string status_message;
      std::swap(status_message, partial_parsed);
      status_message.append(boost::begin(result_range),
                  boost::end(result_range));
      boost::algorithm::trim(status_message);
      status_message_promise.set_value(status_message);
      part_begin = boost::end(result_range);
    } else if (parsed_ok == false) {
#ifdef NETWORK_DEBUG
      std::string escaped;
      debug_escaper escaper(escaped);
      std::for_each(part_begin, part_end, escaper);
      NETWORK_MESSAGE("[parser:"
                  << response_parser_.state()
                  << "] buffer contents: \""
                  << escaped
                  << "\"");
#endif
      std::runtime_error error("Invalid status message part.");
      status_message_promise.set_exception(
        boost::copy_exception(error));
      headers_promise.set_exception(boost::copy_exception(error));
      source_promise.set_exception(boost::copy_exception(error));
      destination_promise.set_exception(boost::copy_exception(error));
      body_promise.set_exception(boost::copy_exception(error));
    } else {
      partial_parsed.append(
        boost::begin(result_range),
        boost::end(result_range));
      part_begin = part.begin();
      connection_delegate_->read_some(
        boost::asio::mutable_buffers_1(part.c_array(), part.size()),
        callback
        );
    }
    return parsed_ok;
  }

  void parse_headers_real(std::string & headers_part) {
     boost::iterator_range< std::string::const_iterator>
      input_range = boost::make_iterator_range(headers_part)
      , result_range;
    boost::logic::tribool parsed_ok;
    response_parser headers_parser(
      response_parser::http_header_line_done);
    std::multimap<std::string, std::string> headers;
    std::pair<std::string,std::string> header_pair;
    while (!boost::empty(input_range)) {
      boost::fusion::tie(parsed_ok, result_range) =
        headers_parser.parse_until(
          response_parser::http_header_colon,
          input_range);
      if (headers_parser.state()
        != response_parser::http_header_colon)
        break;
      header_pair.first = std::string(boost::begin(result_range),
                      boost::end(result_range));
      input_range.advance_begin(boost::distance(result_range));
      boost::fusion::tie(parsed_ok, result_range) =
        headers_parser.parse_until(
          response_parser::http_header_line_done,
          input_range);
      header_pair.second = std::string(boost::begin(result_range),
                       boost::end(result_range));
      input_range.advance_begin(boost::distance(result_range));

      boost::trim(header_pair.first);
      if (header_pair.first.size() > 1) {
        header_pair.first.erase(
          header_pair.first.size() - 1
        );
      }
      boost::trim(header_pair.second);
      headers.insert(header_pair);
    }
    // Set content length
    content_length_ = boost::none;
    auto it = headers.find("Content-Length");
    if (it != headers.end()) {
      try {
        content_length_ = std::stoul(it->second);
        NETWORK_MESSAGE("Content-Length: " << *content_length_);
      } catch(const std::invalid_argument&) {
        NETWORK_MESSAGE("invalid argument exception while interpreting " 
          << it->second << " as content length");
      } catch(const std::out_of_range&) {
        NETWORK_MESSAGE("out of range exception while interpreting " 
          << it->second << " as content length");
      }
    }
    headers_promise.set_value(headers);
  }

  boost::fusion::tuple<boost::logic::tribool, size_t> parse_headers(
      boost::function<void(boost::system::error_code, size_t)> callback,
      size_t bytes) {
    boost::logic::tribool parsed_ok;
    buffer_type::const_iterator part_end = part.begin();
    std::advance(part_end, bytes);
    boost::iterator_range<buffer_type::const_iterator>
      result_range,
      input_range = boost::make_iterator_range(part_begin, part_end);
    boost::fusion::tie(parsed_ok, result_range) = response_parser_.parse_until(
      response_parser::http_headers_done,
      input_range);
    if (parsed_ok == true) {
      std::string headers_string;
      std::swap(headers_string, partial_parsed);
      headers_string.append(boost::begin(result_range),
                  boost::end(result_range));
      part_begin = boost::end(result_range);
      this->parse_headers_real(headers_string);
    } else if (parsed_ok == false) {
      // We want to output the contents of the buffer that caused
      // the error in debug builds.
#ifdef NETWORK_DEBUG
      std::string escaped;
      debug_escaper escaper(escaped);
      std::for_each(part_begin, part_end, escaper);
      NETWORK_MESSAGE("[parser:"
                  << response_parser_.state()
                  << "] buffer contents: \""
                  << escaped
                  << "\" consumed length: "
                  << boost::distance(result_range));
#endif
      std::runtime_error error("Invalid header part.");
      headers_promise.set_exception(boost::copy_exception(error));
      body_promise.set_exception(boost::copy_exception(error));
      source_promise.set_exception(boost::copy_exception(error));
      destination_promise.set_exception(boost::copy_exception(error));
    } else {
      partial_parsed.append(boost::begin(result_range),
                  boost::end(result_range));
      part_begin = part.begin();
      connection_delegate_->read_some(
        boost::asio::mutable_buffers_1(part.c_array(), part.size()),
        callback
        );
    }
    return boost::fusion::make_tuple(
      parsed_ok,
      std::distance(
        boost::end(result_range)
        , part_end
        )
      );
  }

  void parse_body(boost::function<void(boost::system::error_code, size_t)> callback, size_t bytes) {
    // TODO: we should really not use a string for the partial body
    // buffer.
    partial_parsed.append(part_begin, bytes);
    part_begin = part.begin();
    connection_delegate_->read_some(
      boost::asio::mutable_buffers_1(part.c_array(), part.size()),
      callback
      );
  }

  bool follow_redirect_;
  boost::asio::io_service::strand request_strand_;
  boost::shared_ptr<resolver_delegate> resolver_delegate_;
  boost::shared_ptr<connection_delegate> connection_delegate_;
  boost::asio::streambuf command_streambuf;
  std::string method;
  response_parser response_parser_;
  boost::promise<std::string> version_promise;
  boost::promise<boost::uint16_t> status_promise;
  boost::promise<std::string> status_message_promise;
  boost::promise<std::multimap<std::string, std::string> > headers_promise;
  boost::optional<size_t> content_length_;
  boost::promise<std::string> source_promise;
  boost::promise<std::string> destination_promise;
  boost::promise<std::string> body_promise;
  typedef boost::array<char, NETWORK_BUFFER_CHUNK> buffer_type;
  buffer_type part;
  buffer_type::const_iterator part_begin;
  std::string partial_parsed;
  std::string host_;
};

// END OF PIMPL DEFINITION

http_async_connection::http_async_connection(boost::shared_ptr<resolver_delegate> resolver_delegate,
                                             boost::shared_ptr<connection_delegate> connection_delegate,
                                             boost::asio::io_service & io_service,
                                             bool follow_redirects)
: pimpl(new (std::nothrow) http_async_connection_pimpl(resolver_delegate,
                                                       connection_delegate,
                                                       io_service,
                                                       follow_redirects)) {}

http_async_connection::http_async_connection(boost::shared_ptr<http_async_connection_pimpl> new_pimpl)
: pimpl(new_pimpl) {}

http_async_connection::~http_async_connection() {}

http_async_connection * http_async_connection::clone() const {
  boost::shared_ptr<http_async_connection_pimpl> new_pimpl(pimpl->clone());
  return new (std::nothrow) http_async_connection(new_pimpl);
}

response http_async_connection::send_request(std::string const & method,
                                             request const & request,
                                             bool get_body,
                                             callback_type callback,
                                             request_options const &options) {
  return pimpl->start(request, method, get_body, callback, options);
}

void http_async_connection::reset() {
  pimpl->reset();  // NOTE: We're not resetting the pimpl, just the internal state.
}

}  // namespace http
}  // namespace network
  
#endif /* NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_ASYNC_NORMAL_IPP_20111123 */
