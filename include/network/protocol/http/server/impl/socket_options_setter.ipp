// Copyright 2012 Dean Michael Berris <dberris@google.com>.
// Copyright 2012 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef NETWORK_PROTOCOL_HTTP_SERVER_IMPL_SOCKET_OPTIONS_SETTER_IPP_20120319
#define NETWORK_PROTOCOL_HTTP_SERVER_IMPL_SOCKET_OPTIONS_SETTER_IPP_20120319

#include <network/protocol/http/server/impl/socket_options_setter.hpp>
#include <network/protocol/http/server/options.hpp>

namespace network { namespace http {

void socket_options_setter::set_socket_options(server_options const & options, asio::ip::tcp::socket &socket) {
  asio::error_code ignored;
  socket.non_blocking(options.non_blocking_io(), ignored);
  if (options.linger()) {
    asio::ip::tcp::socket::linger linger(true, options.linger_timeout());
    socket.set_option(linger, ignored);
  }
  if (int buf_size = options.receive_buffer_size() >= 0) {
    asio::ip::tcp::socket::receive_buffer_size receive_buffer_size(buf_size);
    socket.set_option(receive_buffer_size, ignored);
  }
  if (int buf_size = options.send_buffer_size() >= 0) {
    asio::ip::tcp::socket::send_buffer_size send_buffer_size(buf_size);
    socket.set_option(send_buffer_size, ignored);
  }
  if (int buf_size = options.receive_low_watermark() >= 0) {
    asio::ip::tcp::socket::receive_low_watermark receive_low_watermark(buf_size);
    socket.set_option(receive_low_watermark, ignored);
  }
  if (int buf_size = options.send_low_watermark() >= 0) {
    asio::ip::tcp::socket::send_low_watermark send_low_watermark(buf_size);
    socket.set_option(send_low_watermark, ignored);
  }
}

void socket_options_setter::set_acceptor_options(server_options const &options, asio::ip::tcp::acceptor &acceptor) {
  asio::error_code ignored;
  acceptor.set_option(
      asio::ip::tcp::acceptor::reuse_address(options.reuse_address()),
      ignored);
  acceptor.set_option(
      asio::ip::tcp::acceptor::enable_connection_aborted(options.report_aborted()),
      ignored);
}

}  // namespace http
}  // namespace network

#endif  // NETWORK_PROTOCOL_HTTP_SERVER_IMPL_SOCKET_OPTIONS_SETTER_IPP_20120319
