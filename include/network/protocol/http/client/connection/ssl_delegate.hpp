// Copyright 2011 Dean Michael Berris (dberris@google.com).
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_SSL_DELEGATE_20110819
#define NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_SSL_DELEGATE_20110819

#include <boost/asio/ssl.hpp>
#include <network/protocol/http/client/connection/connection_delegate.hpp>
#include <network/protocol/http/client/options.hpp>
#include <boost/enable_shared_from_this.hpp>

namespace boost {
namespace asio {
class io_service;
}  // namespace asio
}  // namespace boost

namespace network {
namespace http {

struct ssl_delegate : connection_delegate, boost::enable_shared_from_this<ssl_delegate> {
  ssl_delegate(boost::asio::io_service & service,
               client_options const &options);

  virtual void connect(boost::asio::ip::tcp::endpoint & endpoint,
                       std::string const &host,
                       boost::function<void(boost::system::error_code const &)> handler);
  virtual void write(boost::asio::streambuf & command_streambuf,
                     boost::function<void(boost::system::error_code const &, size_t)> handler);
  virtual void read_some(boost::asio::mutable_buffers_1 const & read_buffer,
                         boost::function<void(boost::system::error_code const &, size_t)> handler);
  ~ssl_delegate();

 private:
  boost::asio::io_service & service_;
  client_options options_;
  boost::scoped_ptr<boost::asio::ssl::context> context_;
  boost::scoped_ptr<boost::asio::ssl::stream<boost::asio::ip::tcp::socket> > socket_;

  ssl_delegate(ssl_delegate const &);  // = delete
  ssl_delegate& operator=(ssl_delegate);  // = delete

  void handle_connected(boost::system::error_code const & ec,
                        boost::function<void(boost::system::error_code const &)> handler);
};

}  // namespace http
}  // namespace network

#endif /* NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_SSL_DELEGATE_20110819 */
