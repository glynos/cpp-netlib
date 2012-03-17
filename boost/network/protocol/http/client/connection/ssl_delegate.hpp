#ifndef BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_SSL_DELEGATE_20110819
#define BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_SSL_DELEGATE_20110819

// Copyright 2011 Dean Michael Berris (dberris@google.com).
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/asio/ssl.hpp>
#include <boost/network/protocol/http/client/connection/connection_delegate.hpp>
#include <boost/network/protocol/http/client/options.hpp>
#include <boost/enable_shared_from_this.hpp>

namespace boost { namespace asio {

class io_service;

}  // namespace asio

}  // namespace boost

namespace boost { namespace network { namespace http {

struct ssl_delegate : connection_delegate, enable_shared_from_this<ssl_delegate> {
  ssl_delegate(asio::io_service & service,
               client_options const &options);

  virtual void connect(asio::ip::tcp::endpoint & endpoint,
                       function<void(system::error_code const &)> handler);
  virtual void write(asio::streambuf & command_streambuf,
                     function<void(system::error_code const &, size_t)> handler);
  virtual void read_some(asio::mutable_buffers_1 const & read_buffer,
                         function<void(system::error_code const &, size_t)> handler);
  ~ssl_delegate();

 private:
  asio::io_service & service_;
  client_options options_;
  scoped_ptr<asio::ssl::context> context_;
  scoped_ptr<asio::ssl::stream<asio::ip::tcp::socket> > socket_;

  ssl_delegate(ssl_delegate const &);  // = delete
  ssl_delegate& operator=(ssl_delegate);  // = delete

  void handle_connected(system::error_code const & ec,
                        function<void(system::error_code const &)> handler);
};

} /* http */

} /* network */

} /* boost */

#endif /* BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_SSL_DELEGATE_20110819 */
