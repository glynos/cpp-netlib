#ifndef BOOST_NETWORK_PROTOCOL_HTTP_IMPL_HTTP_ASYNC_CONNECTION_HPP_20100601
#define BOOST_NETWORK_PROTOCOL_HTTP_IMPL_HTTP_ASYNC_CONNECTION_HPP_20100601

// Copyright 2010 (C) Dean Michael Berris
// Copyright 2010 (C) Sinefunc, Inc.
// Copyright 2011 Dean Michael Berris (dberris@google.com).
// Copyright 2011 Google,Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <iterator>
#include <cstdint>
#include <boost/algorithm/string/trim.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/asio/strand.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/assert.hpp>
#include <boost/logic/tribool.hpp>
#include <boost/network/constants.hpp>
#include <boost/network/detail/debug.hpp>
#include <boost/network/protocol/http/algorithms/linearize.hpp>
#include <boost/network/protocol/http/client/connection/async_protocol_handler.hpp>
#include <boost/network/protocol/http/message/wrappers/host.hpp>
#include <boost/network/protocol/http/message/wrappers/uri.hpp>
#include <boost/network/protocol/http/parser/incremental.hpp>
#include <boost/network/protocol/http/traits/delegate_factory.hpp>
#include <boost/network/traits/istream.hpp>
#include <boost/network/traits/ostream_iterator.hpp>
#include <boost/network/version.hpp>
#include <boost/range/algorithm/transform.hpp>
#include <boost/throw_exception.hpp>

namespace boost {
namespace network {
namespace http {
namespace impl {

template <class Tag, unsigned version_major, unsigned version_minor>
struct async_connection_base;

namespace placeholders = boost::asio::placeholders;

template <class Tag, unsigned version_major, unsigned version_minor>
struct http_async_connection
    : async_connection_base<Tag, version_major, version_minor>,
      protected http_async_protocol_handler<Tag, version_major, version_minor>,
      std::enable_shared_from_this<
          http_async_connection<Tag, version_major, version_minor> > {
  http_async_connection(http_async_connection const&) = delete;

  typedef async_connection_base<Tag, version_major, version_minor> base;
  typedef http_async_protocol_handler<Tag, version_major, version_minor>
      protocol_base;
  typedef typename base::resolver_type resolver_type;
  typedef typename base::resolver_base::resolver_iterator resolver_iterator;
  typedef typename base::resolver_base::resolver_iterator_pair
      resolver_iterator_pair;
  typedef typename base::response response;
  typedef typename base::string_type string_type;
  typedef typename base::request request;
  typedef typename base::resolver_base::resolve_function resolve_function;
  typedef
      typename base::body_callback_function_type body_callback_function_type;
  typedef
      typename base::body_generator_function_type body_generator_function_type;
  typedef http_async_connection<Tag, version_major, version_minor> this_type;
  typedef typename delegate_factory<Tag>::type delegate_factory_type;
  typedef typename delegate_factory_type::connection_delegate_ptr
      connection_delegate_ptr;

  http_async_connection(resolver_type& resolver, resolve_function resolve,
                        bool follow_redirect, int timeout,
                        connection_delegate_ptr delegate)
      : timeout_(timeout),
        timer_(resolver.get_io_service()),
        is_timedout_(false),
        follow_redirect_(follow_redirect),
        resolver_(resolver),
        resolve_(std::move(resolve)),
        request_strand_(resolver.get_io_service()),
        delegate_(std::move(delegate)) {}

  // This is the main entry point for the connection/request pipeline.
  // We're
  // overriding async_connection_base<...>::start(...) here which is
  // called
  // by the client.
  virtual response start(request const& request, string_type const& method,
                         bool get_body, body_callback_function_type callback,
                         body_generator_function_type generator) {
    response response_;
    this->init_response(response_, get_body);
    linearize(request, method, version_major, version_minor,
              std::ostreambuf_iterator<typename char_<Tag>::type>(
                  &command_streambuf));
    this->method = method;
    std::uint16_t port_ = port(request);
    string_type host_ = host(request);
    std::uint16_t source_port = request.source_port();

    auto self = this->shared_from_this();
    resolve_(resolver_, host_, port_,
             request_strand_.wrap(
                                  [=] (boost::system::error_code const &ec,
                                       resolver_iterator_pair endpoint_range) {
                                    self->handle_resolved(host_, port_, source_port, get_body,
                                                          callback, generator, ec, endpoint_range);
                                  }));
    if (timeout_ > 0) {
      timer_.expires_from_now(std::chrono::seconds(timeout_));
      timer_.async_wait(request_strand_.wrap([=] (boost::system::error_code const &ec) {
            self->handle_timeout(ec);
          }));
    }
    return response_;
  }

 private:
  void set_errors(boost::system::error_code const& ec, body_callback_function_type callback) {
    boost::system::system_error error(ec);
    this->version_promise.set_exception(std::make_exception_ptr(error));
    this->status_promise.set_exception(std::make_exception_ptr(error));
    this->status_message_promise.set_exception(std::make_exception_ptr(error));
    this->headers_promise.set_exception(std::make_exception_ptr(error));
    this->source_promise.set_exception(std::make_exception_ptr(error));
    this->destination_promise.set_exception(std::make_exception_ptr(error));
    this->body_promise.set_exception(std::make_exception_ptr(error));
    if ( callback )
      callback(boost::iterator_range<typename std::array<typename char_<Tag>::type, 1024>::const_iterator>(), ec);
    this->timer_.cancel();
  }

  void handle_timeout(boost::system::error_code const& ec) {
    if (!ec) delegate_->disconnect();
    is_timedout_ = true;
  }

  void handle_resolved(string_type host, std::uint16_t port,
                       std::uint16_t source_port, bool get_body,
                       body_callback_function_type callback,
                       body_generator_function_type generator,
                       boost::system::error_code const& ec,
                       resolver_iterator_pair endpoint_range) {
    if (!ec && !boost::empty(endpoint_range)) {
      // Here we deal with the case that there was an error encountered and
      // that there's still more endpoints to try connecting to.
      resolver_iterator iter = boost::begin(endpoint_range);
      boost::asio::ip::tcp::endpoint endpoint(iter->endpoint().address(), port);
      auto self = this->shared_from_this();
      delegate_->connect(
          endpoint, host, source_port,
          request_strand_.wrap([=] (boost::system::error_code const &ec) {
              auto iter_copy = iter;
              self->handle_connected(host, port, source_port, get_body, callback,
                                     generator, std::make_pair(++iter_copy, resolver_iterator()), ec);
            }));
    } else {
      set_errors((ec ? ec : boost::asio::error::host_not_found), callback);
    }
  }

  void handle_connected(string_type host, std::uint16_t port,
                        std::uint16_t source_port, bool get_body,
                        body_callback_function_type callback,
                        body_generator_function_type generator,
                        resolver_iterator_pair endpoint_range,
                        boost::system::error_code const& ec) {
    if (is_timedout_) {
      set_errors(boost::asio::error::timed_out, callback);
    } else if (!ec) {
      BOOST_ASSERT(delegate_.get() != 0);
      auto self = this->shared_from_this();
      delegate_->write(
          command_streambuf,
          request_strand_.wrap([=] (boost::system::error_code const &ec,
                                    std::size_t bytes_transferred) {
                                 self->handle_sent_request(get_body, callback, generator,
                                                           ec, bytes_transferred);
                               }));
    } else {
      if (!boost::empty(endpoint_range)) {
        resolver_iterator iter = boost::begin(endpoint_range);
        boost::asio::ip::tcp::endpoint endpoint(iter->endpoint().address(), port);
        auto self = this->shared_from_this();
        delegate_->connect(
            endpoint, host, source_port,
            request_strand_.wrap([=] (boost::system::error_code const &ec) {
                auto iter_copy = iter;
                self->handle_connected(host, port, source_port, get_body, callback,
                                       generator, std::make_pair(++iter_copy, resolver_iterator()),
                                       ec);
              }));
      } else {
        set_errors((ec ? ec : boost::asio::error::host_not_found), callback);
      }
    }
  }

  enum state_t { version, status, status_message, headers, body };

  void handle_sent_request(bool get_body, body_callback_function_type callback,
                           body_generator_function_type generator,
                           boost::system::error_code const& ec,
                           std::size_t bytes_transferred) {
    if (!is_timedout_ && !ec) {
      if (generator) {
        // Here we write some more data that the generator provides, before we
        // wait for data from the server.
        string_type chunk;
        if (generator(chunk)) {
          // At this point this means we have more data to write, so we write
          // it out.
          std::copy(chunk.begin(), chunk.end(),
                    std::ostreambuf_iterator<typename char_<Tag>::type>(
                        &command_streambuf));
          auto self = this->shared_from_this();
          delegate_->write(
              command_streambuf,
              request_strand_.wrap([=] (boost::system::error_code const &ec,
                                        std::size_t bytes_transferred) {
                                     self->handle_sent_request(get_body, callback, generator,
                                                               ec, bytes_transferred);
                                   }));
          return;
        }
      }

      auto self = this->shared_from_this();
      delegate_->read_some(
          boost::asio::mutable_buffers_1(this->part.data(),
                                         this->part.size()),
          request_strand_.wrap([=] (boost::system::error_code const &ec,
                                    std::size_t bytes_transferred) {
                                 self->handle_received_data(version, get_body, callback,
                                                            ec, bytes_transferred);
                               }));
    } else {
      set_errors((is_timedout_ ? boost::asio::error::timed_out : ec), callback);
    }
  }

  void handle_received_data(state_t state, bool get_body,
                            body_callback_function_type callback,
                            boost::system::error_code const& ec,
                            std::size_t bytes_transferred) {
    static const long short_read_error = 335544539;
    bool is_ssl_short_read_error =
#ifdef BOOST_NETWORK_ENABLE_HTTPS
        ec.category() == boost::asio::error::ssl_category &&
        ec.value() == short_read_error;
#else
        false && short_read_error;
#endif
    if (!is_timedout_ &&
        (!ec || ec == boost::asio::error::eof || is_ssl_short_read_error)) {
      logic::tribool parsed_ok;
      size_t remainder;
      auto self = this->shared_from_this();
      switch (state) {
        case version:
          if (ec == boost::asio::error::eof) return;
          parsed_ok = this->parse_version(
              delegate_,
              request_strand_.wrap([=] (boost::system::error_code const &ec,
                                        std::size_t bytes_transferred) {
                                     self->handle_received_data(version, get_body, callback,
                                                                ec, bytes_transferred);
                                   }),
              bytes_transferred);
          if (!parsed_ok || indeterminate(parsed_ok)) {
            return;
          }
        case status:
          if (ec == boost::asio::error::eof) return;
          parsed_ok = this->parse_status(
              delegate_,
              request_strand_.wrap([=] (boost::system::error_code const &ec,
                                        std::size_t bytes_transferred) {
                                     self->handle_received_data(status, get_body, callback,
                                                                ec, bytes_transferred);
                                   }),
              bytes_transferred);
          if (!parsed_ok || indeterminate(parsed_ok)) {
            return;
          }
        case status_message:
          if (ec == boost::asio::error::eof) return;
          parsed_ok = this->parse_status_message(
              delegate_, request_strand_.wrap([=] (boost::system::error_code const &,
                                                   std::size_t bytes_transferred) {
                                                self->handle_received_data(status_message, get_body, callback,
                                                                           ec, bytes_transferred);
                                              }),
              bytes_transferred);
          if (!parsed_ok || indeterminate(parsed_ok)) {
            return;
          }
        case headers:
          if (ec == boost::asio::error::eof) return;
          // In the following, remainder is the number of bytes that remain in
          // the buffer. We need this in the body processing to make sure that
          // the data remaining in the buffer is dealt with before another call
          // to get more data for the body is scheduled.
          std::tie(parsed_ok, remainder) = this->parse_headers(
              delegate_,
              request_strand_.wrap([=] (boost::system::error_code const &ec,
                                        std::size_t bytes_transferred) {
                                     self->handle_received_data(headers, get_body, callback,
                                                                ec, bytes_transferred);
                                   }),
              bytes_transferred);

          if (!parsed_ok || indeterminate(parsed_ok)) {
            return;
          }

          if (!get_body) {
            // We short-circuit here because the user does not want to get the
            // body (in the case of a HEAD request).
            this->body_promise.set_value("");
            if ( callback )
              callback(boost::iterator_range<typename std::array<typename char_<Tag>::type, 1024>::const_iterator>(), ::asio::error::eof);
            this->destination_promise.set_value("");
            this->source_promise.set_value("");
            // this->part.assign('\0');
            boost::copy("\0", std::begin(this->part));
            this->response_parser_.reset();
            return;
          }

          if (callback) {
            // Here we deal with the spill-over data from the headers
            // processing. This means the headers data has already been parsed
            // appropriately and we're looking to treat everything that remains
            // in the buffer.
            typename protocol_base::buffer_type::const_iterator begin =
                this->part_begin;
            typename protocol_base::buffer_type::const_iterator end = begin;
            std::advance(end, remainder);

            // We're setting the body promise here to an empty string because
            // this can be used as a signaling mechanism for the user to
            // determine that the body is now ready for processing, even though
            // the callback is already provided.
            this->body_promise.set_value("");

            // The invocation of the callback is synchronous to allow us to
            // wait before scheduling another read.
            callback(make_iterator_range(begin, end), ec);

            auto self = this->shared_from_this();
            delegate_->read_some(
                boost::asio::mutable_buffers_1(this->part.data(),
                                               this->part.size()),
                request_strand_.wrap([=] (boost::system::error_code const &ec,
                                          std::size_t bytes_transferred) {
                                       self->handle_received_data(body, get_body, callback,
                                                                  ec, bytes_transferred);
                                     }));
          } else {
            // Here we handle the body data ourself and append to an
            // ever-growing string buffer.
            auto self = this->shared_from_this();
            this->parse_body(
                delegate_,
                request_strand_.wrap([=] (boost::system::error_code const &ec,
                                          std::size_t bytes_transferred) {
                                       self->handle_received_data(body, get_body, callback,
                                                                  ec, bytes_transferred);
                                     }),
                remainder);
          }
          return;
        case body:
          if (ec == boost::asio::error::eof || is_ssl_short_read_error) {
            // Here we're handling the case when the connection has been closed
            // from the server side, or at least that the end of file has been
            // reached while reading the socket. This signals the end of the
            // body processing chain.
            if (callback) {
              typename protocol_base::buffer_type::const_iterator
                  begin = this->part.begin(),
                  end = begin;
              std::advance(end, bytes_transferred);

              // We call the callback function synchronously passing the error
              // condition (in this case, end of file) so that it can handle it
              // appropriately.
              callback(make_iterator_range(begin, end), ec);
            } else {
              string_type body_string;
              std::swap(body_string, this->partial_parsed);
              body_string.append(this->part.begin(), this->part.begin() + bytes_transferred);
              if (this->is_chunk_encoding) {
                this->body_promise.set_value(parse_chunk_encoding(body_string));
              } else {
                this->body_promise.set_value(body_string);
              }
            }
            // TODO(dberris): set the destination value somewhere!
            this->destination_promise.set_value("");
            this->source_promise.set_value("");
            // this->part.assign('\0');
            boost::copy("\0", std::begin(this->part));
            this->response_parser_.reset();
            this->timer_.cancel();
          } else {
            // This means the connection has not been closed yet and we want to
            // get more data.
            if (callback) {
              // Here we have a body_handler callback. Let's invoke the
              // callback from here and make sure we're getting more data right
              // after.
              typename protocol_base::buffer_type::const_iterator begin =
                  this->part.begin();
              typename protocol_base::buffer_type::const_iterator end = begin;
              std::advance(end, bytes_transferred);
              callback(make_iterator_range(begin, end), ec);
              auto self = this->shared_from_this();
              delegate_->read_some(
                  boost::asio::mutable_buffers_1(this->part.data(),
                                                 this->part.size()),
                  request_strand_.wrap([=] (boost::system::error_code const &ec,
                                            std::size_t bytes_transferred) {
                                         self->handle_received_data(body, get_body, callback,
                                                                    ec, bytes_transferred);
                                       }));
            } else {
              // Here we don't have a body callback. Let's make sure that we
              // deal with the remainder from the headers part in case we do
              // have data that's still in the buffer.
              this->parse_body(
                  delegate_,
                  request_strand_.wrap([=] (boost::system::error_code const &ec,
                                            std::size_t bytes_transferred) {
                                         self->handle_received_data(body, get_body, callback,
                                                                    ec, bytes_transferred);
                                       }),
                  bytes_transferred);
            }
          }
          return;
        default:
          BOOST_ASSERT(false && "Bug, report this to the developers!");
      }
    } else {
      boost::system::error_code report_code = is_timedout_ ? boost::asio::error::timed_out : ec;
      boost::system::system_error error(report_code);
      this->source_promise.set_exception(std::make_exception_ptr(error));
      this->destination_promise.set_exception(std::make_exception_ptr(error));
      switch (state) {
        case version:
          this->version_promise.set_exception(std::make_exception_ptr(error));
        case status:
          this->status_promise.set_exception(std::make_exception_ptr(error));
        case status_message:
          this->status_message_promise.set_exception(
              std::make_exception_ptr(error));
        case headers:
          this->headers_promise.set_exception(std::make_exception_ptr(error));
        case body:
          if (!callback) {
            // N.B. if callback is non-null, then body_promise has already been
            // set to value "" to indicate body is handled by streaming handler
            // so no exception should be set
            this->body_promise.set_exception(std::make_exception_ptr(error));
          }
          else
            callback(boost::iterator_range<typename std::array<typename char_<Tag>::type, 1024>::const_iterator>(), report_code);
          break;
        default:
          BOOST_ASSERT(false && "Bug, report this to the developers!");
      }
    }
  }

  string_type parse_chunk_encoding(string_type& body_string) {
    string_type body;
    string_type crlf = "\r\n";

    typename string_type::iterator begin = body_string.begin();
    for (typename string_type::iterator iter =
             std::search(begin, body_string.end(), crlf.begin(), crlf.end());
         iter != body_string.end();
         iter =
             std::search(begin, body_string.end(), crlf.begin(), crlf.end())) {
      string_type line(begin, iter);
      if (line.empty()) {
        break;
      }
      std::stringstream stream(line);
      int len;
      stream >> std::hex >> len;
      std::advance(iter, 2);
      if (len == 0) {
        break;
      }
      if (len <= body_string.end() - iter) {
        body.insert(body.end(), iter, iter + len);
        std::advance(iter, len + 2);
      }
      begin = iter;
    }

    return body;
  }

  int timeout_;
  boost::asio::steady_timer timer_;
  bool is_timedout_;
  bool follow_redirect_;
  resolver_type& resolver_;
  resolve_function resolve_;
  boost::asio::io_service::strand request_strand_;
  connection_delegate_ptr delegate_;
  boost::asio::streambuf command_streambuf;
  string_type method;
};

}  // namespace impl
}  // namespace http
}  // namespace network
}  // namespace boost

#endif  // BOOST_NETWORK_PROTOCOL_HTTP_IMPL_HTTP_ASYNC_CONNECTION_HPP_20100601
