#ifndef BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_NORMAL_DELEGATE_IPP_20110819
#define BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_NORMAL_DELEGATE_IPP_20110819

// Copyright 2011 Dean Michael Berris (dberris@google.com).
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <cstdint>
#include <functional>
#include <asio/ip/tcp.hpp>
#include <asio/streambuf.hpp>
#include <asio/write.hpp>
#include <asio/buffer.hpp>
#include <boost/network/protocol/http/client/connection/normal_delegate.hpp>

boost::network::http::impl::normal_delegate::normal_delegate(
    asio::io_service &service)
    : service_(service) {}

void boost::network::http::impl::normal_delegate::connect(
    asio::ip::tcp::endpoint &endpoint, std::string host,
    std::uint16_t source_port,
    std::function<void(std::error_code const &)> handler) {

  // TODO(dberris): review parameter necessity.
  (void)host;

  socket_.reset(new asio::ip::tcp::socket(
      service_,
      asio::ip::tcp::endpoint(asio::ip::address(), source_port)));
  socket_->async_connect(endpoint, handler);
}

void boost::network::http::impl::normal_delegate::write(
    asio::streambuf &command_streambuf,
    std::function<void(std::error_code const &, size_t)> handler) {
  asio::async_write(*socket_, command_streambuf, handler);
}

void boost::network::http::impl::normal_delegate::read_some(
    asio::mutable_buffers_1 const &read_buffer,
    std::function<void(std::error_code const &, size_t)> handler) {
  socket_->async_read_some(read_buffer, handler);
}

void boost::network::http::impl::normal_delegate::disconnect() {
  if (socket_.get() && socket_->is_open()) {
    std::error_code ignored;
    socket_->shutdown(asio::ip::tcp::socket::shutdown_both, ignored);
    if (!ignored) {
      socket_->close(ignored);
    }
  }
}

#endif  // BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_NORMAL_DELEGATE_IPP_20110819
