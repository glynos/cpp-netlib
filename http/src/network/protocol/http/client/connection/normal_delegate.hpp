// Copyright 2011 Dean Michael Berris (dberris@google.com).
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_NORMAL_DELEGATE_20110819
#define NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_NORMAL_DELEGATE_20110819

#include <memory>
#include <network/protocol/http/client/connection/connection_delegate.hpp>

namespace boost { namespace asio {

class io_service;

}  // namespace asio

}  // namespace boost

namespace network {
namespace http {

struct normal_delegate : connection_delegate {
  normal_delegate(boost::asio::io_service & service);

  virtual void connect(boost::asio::ip::tcp::endpoint & endpoint,
                       std::string const &host,
                       std::function<void(boost::system::error_code const &)> handler);
  virtual void write(boost::asio::streambuf & command_streambuf,
                     std::function<void(boost::system::error_code const &, size_t)> handler);
  virtual void read_some(boost::asio::mutable_buffers_1 const & read_buffer,
                         std::function<void(boost::system::error_code const &, size_t)> handler);
  ~normal_delegate();

 private:
  boost::asio::io_service & service_;
  std::unique_ptr<boost::asio::ip::tcp::socket> socket_;

  normal_delegate(normal_delegate const &);  // = delete
  normal_delegate& operator=(normal_delegate);  // = delete
};

}  // namespace http
}  // namespace network

#endif /* NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_NORMAL_DELEGATE_20110819 */
