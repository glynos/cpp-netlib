#ifndef BOOST_NETWORK_PROTOCOL_HTTP_IMPL_HTTP_ASYNC_CONNECTION_HPP_20100601
#define BOOST_NETWORK_PROTOCOL_HTTP_IMPL_HTTP_ASYNC_CONNECTION_HPP_20100601

// Copyright 2010 (C) Dean Michael Berris
// Copyright 2010 (C) Sinefunc, Inc.
// Copyright 2011 Dean Michael Berris (dberris@google.com).
// Copyright 2011 Google,Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/network/protocol/http/client/client_connection.hpp>
#include <boost/enable_shared_from_this.hpp>

namespace boost { namespace asio {

class io_service;

}  // namespace asio

}  // namespace boost

namespace boost { namespace network { namespace http {

struct request;
struct response;
struct resolver_delegate;
struct connection_delegate;

struct http_async_connection_pimpl;

struct http_async_connection : client_connection
                             , enable_shared_from_this<http_async_connection> {
  using client_connection::callback_type;
  http_async_connection(shared_ptr<resolver_delegate> resolver_delegate,
                        shared_ptr<connection_delegate> connection_delegate,
                        asio::io_service & io_service,
                        bool follow_redirects);
  http_async_connection * clone() const;
  virtual response send_request(std::string const & method,
                                request const & request,
                                bool get_body,
                                callback_type callback,
                                request_options const &options);  // override
  virtual void reset();  // override
  virtual ~http_async_connection();
 private:
  explicit http_async_connection(shared_ptr<http_async_connection_pimpl>);
  shared_ptr<http_async_connection_pimpl> pimpl;
};

} // namespace http

} // namespace network

} // namespace boost

#endif // BOOST_NETWORK_PROTOCOL_HTTP_IMPL_HTTP_ASYNC_CONNECTION_HPP_20100601
