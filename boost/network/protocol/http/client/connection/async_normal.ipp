#ifndef BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_ASYNC_NORMAL_IPP_20111123
#define BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_ASYNC_NORMAL_IPP_20111123

// Copyright 2011 Dean Michael Berris <dberris@google.com>.
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/asio/placeholders.hpp>
#include <boost/network/protocol/http/client/connection/async_normal.hpp>
#include <boost/network/protocol/http/algorithms/linearize.hpp>
#include <boost/asio/strand.hpp>

namespace boost { namespace network { namespace http {

namespace placeholders = boost::asio::placeholders;

struct http_async_connection_pimpl : boost::enable_shared_from_this<http_async_connection_pimpl>
{
  typedef http_async_connection::callback_type body_callback_function_type;
  typedef resolver_delegate::resolver_iterator resolver_iterator;
  typedef resolver_delegate::iterator_pair resolver_iterator_pair;

  http_async_connection_pimpl(
    shared_ptr<resolver_delegate> resolver_delegate,
    shared_ptr<connection_delegate> connection_delegate,
    asio::io_service & io_service,
    bool follow_redirect)
  :
        follow_redirect_(follow_redirect),
        request_strand_(io_service),
        resolver_delegate_(resolver_delegate),
        connection_delegate_(connection_delegate) {}

  // This is the main entry point for the connection/request pipeline. We're
  // overriding async_connection_base<...>::start(...) here which is called
  // by the client.
  response start(request const & request,
                 std::string const & method,
                 bool get_body,
                 body_callback_function_type callback) {
    response response_;
    linearize(request, method, 1, 1,
      std::ostreambuf_iterator<char>(&command_streambuf));
    this->method = method;
    boost::uint16_t port_ = port(request);
    resolver_delegate_->resolve(
        host(request),
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

 private:

  http_async_connection_pimpl(http_async_connection_pimpl const &); // = delete

  void set_errors(boost::system::error_code const & ec) {
    boost::system::system_error error(ec);
    // FIXME: Wire up the response object for errors.
//    this->version_promise.set_exception(boost::copy_exception(error));
//    this->status_promise.set_exception(boost::copy_exception(error));
//    this->status_message_promise.set_exception(boost::copy_exception(error));
//    this->headers_promise.set_exception(boost::copy_exception(error));
//    this->source_promise.set_exception(boost::copy_exception(error));
//    this->destination_promise.set_exception(boost::copy_exception(error));
//    this->body_promise.set_exception(boost::copy_exception(error));
  }

  void handle_resolved(boost::uint16_t port,
                       bool get_body,
                       body_callback_function_type callback,
                       boost::system::error_code const & ec,
                       resolver_iterator_pair endpoint_range) {
    if (!ec && !boost::empty(endpoint_range)) {
      // Here we deal with the case that there was an error encountered and
      // that there's still more endpoints to try connecting to.
      resolver_iterator iter = boost::begin(endpoint_range);
      asio::ip::tcp::endpoint endpoint(iter->endpoint().address(), port);
      connection_delegate_->connect(endpoint,
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

  void handle_connected(boost::uint16_t port,
                        bool get_body,
                        body_callback_function_type callback,
                        resolver_iterator_pair endpoint_range,
                        boost::system::error_code const & ec) {
    if (!ec) {
      BOOST_ASSERT(connection_delegate_.get() != 0);
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
      if (!boost::empty(endpoint_range)) {
        resolver_iterator iter = boost::begin(endpoint_range);
        asio::ip::tcp::endpoint endpoint(iter->endpoint().address(), port);
        connection_delegate_->connect(endpoint,
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
    if (!ec) {
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
      set_errors(ec);
    }
  }

  void handle_received_data(state_t state, bool get_body, body_callback_function_type callback, boost::system::error_code const & ec, std::size_t bytes_transferred) {
    if (!ec || ec == boost::asio::error::eof) {
      logic::tribool parsed_ok;
      size_t remainder;
      switch(state) {
        case version:
          parsed_ok =
              this->parse_version(connection_delegate_,
                                  request_strand_.wrap(
                                      boost::bind(
                                          &this_type::handle_received_data,
                                          this_type::shared_from_this(),
                                          version, get_body, callback,
                                          placeholders::error,
                                          placeholders::bytes_transferred)),
                                  bytes_transferred);
          if (!parsed_ok || indeterminate(parsed_ok)) return;
        case status:
          parsed_ok =
              this->parse_status(connection_delegate_,
                                 request_strand_.wrap(
                                     boost::bind(
                                         &this_type::handle_received_data,
                                         this_type::shared_from_this(),
                                         status, get_body, callback,
                                         placeholders::error,
                                         placeholders::bytes_transferred)),
                                 bytes_transferred);
          if (!parsed_ok || indeterminate(parsed_ok)) return;
        case status_message:
          parsed_ok =
            this->parse_status_message(connection_delegate_,
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
          // In the following, remainder is the number of bytes that remain
          // in the buffer. We need this in the body processing to make sure
          // that the data remaining in the buffer is dealt with before
          // another call to get more data for the body is scheduled.
          fusion::tie(parsed_ok, remainder) =
            this->parse_headers(connection_delegate_,
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
            // We short-circuit here because the user does not
            // want to get the body (in the case of a HEAD
            // request).
            this->body_promise.set_value("");
            this->destination_promise.set_value("");
            this->source_promise.set_value("");
            this->part.assign('\0');
            this->response_parser_.reset();
            return;
          }

          if (callback) {
            // Here we deal with the spill-over data from the
            // headers processing. This means the headers data
            // has already been parsed appropriately and we're
            // looking to treat everything that remains in the
            // buffer.
            typename protocol_base::buffer_type::const_iterator begin = this->part_begin;
            typename protocol_base::buffer_type::const_iterator end = begin;
            std::advance(end, remainder);

            // We're setting the body promise here to an empty string because
            // this can be used as a signaling mechanism for the user to
            // determine that the body is now ready for processing, even
            // though the callback is already provided.
            this->body_promise.set_value("");

            // The invocation of the callback is synchronous to allow us to
            // wait before scheduling another read.
            callback(make_iterator_range(begin, end), ec);

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
            // Here we handle the body data ourself and append to an
            // ever-growing string buffer.
            this->parse_body(
              connection_delegate_,
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
          if (ec == boost::asio::error::eof) {
            // Here we're handling the case when the connection has been
            // closed from the server side, or at least that the end of file
            // has been reached while reading the socket. This signals the end
            // of the body processing chain.
            if (callback) {
              typename protocol_base::buffer_type::const_iterator begin =
                  this->part.begin(),
                  end = begin;
              std::advance(end, bytes_transferred);

              // We call the callback function synchronously passing the error
              // condition (in this case, end of file) so that it can handle
              // it appropriately.
              callback(make_iterator_range(begin, end), ec);
            } else {
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
            // This means the connection has not been closed yet and we want to get more
            // data.
            if (callback) {
              // Here we have a body_handler callback. Let's invoke the
              // callback from here and make sure we're getting more data
              // right after.
              typename protocol_base::buffer_type::const_iterator begin = this->part.begin();
              typename protocol_base::buffer_type::const_iterator end = begin;
              std::advance(end, bytes_transferred);
              callback(make_iterator_range(begin, end), ec);
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
              // Here we don't have a body callback. Let's
              // make sure that we deal with the remainder
              // from the headers part in case we do have data
              // that's still in the buffer.
              this->parse_body(connection_delegate_,
                               request_strand_.wrap(
                                   boost::bind(
                                       &this_type::handle_received_data,
                                       this_type::shared_from_this(),
                                       body,
                                       get_body,
                                       callback,
                                       placeholders::error,
                                       placeholders::bytes_transferred)),
                               bytes_transferred);
            }
          }
          return;
        default:
          BOOST_ASSERT(false && "Bug, report this to the developers!");
      }
    } else {
      boost::system::system_error error(ec);
      this->source_promise.set_exception(boost::copy_exception(error));
      this->destination_promise.set_exception(boost::copy_exception(error));
      switch (state) {
        case version:
//          this->version_promise.set_exception(boost::copy_exception(error));
        case status:
//          this->status_promise.set_exception(boost::copy_exception(error));
        case status_message:
//          this->status_message_promise.set_exception(boost::copy_exception(error));
        case headers:
//          this->headers_promise.set_exception(boost::copy_exception(error));
        case body:
//          this->body_promise.set_exception(boost::copy_exception(error));
//          break;
        default:
          BOOST_ASSERT(false && "Bug, report this to the developers!");
      }
    }
  }

  bool follow_redirect_;
  boost::asio::io_service::strand request_strand_;
  shared_ptr<resolver_delegate> resolver_delegate_;
  shared_ptr<connection_delegate> connection_delegate_;
  boost::asio::streambuf command_streambuf;
  std::string method;
};

// END OF PIMPL DEFINITION

http_async_connection::http_async_connection(shared_ptr<resolver_delegate> resolver_delegate,
                                             shared_ptr<connection_delegate> connection_delegate,
                                             asio::io_service & io_service,
                                             bool follow_redirects)
: pimpl(new (std::nothrow) http_async_connection_pimpl(resolver_delegate,
                                                       connection_delegate,
                                                       io_service,
                                                       follow_redirects))
{}

http_async_connection::http
  
} /* http */
  
} /* network */
  
} /* boost */

#endbooif /* BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_ASYNC_NORMAL_IPP_20111123 */
