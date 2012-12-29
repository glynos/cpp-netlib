// Copyright 2011 Dean Michael Berris (dberris@google.com).
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_NORMAL_DELEGATE_IPP_20110819
#define NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_NORMAL_DELEGATE_IPP_20110819

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/asio/write.hpp>
#include <functional>
#include <boost/asio/buffer.hpp>
#include <network/protocol/http/client/connection/normal_delegate.hpp>
#include <network/detail/debug.hpp>

network::http::normal_delegate::normal_delegate(boost::asio::io_service & service)
: service_(service)
{}

void network::http::normal_delegate::connect(boost::asio::ip::tcp::endpoint & endpoint,
                                             std::string const &host,
                                             std::function<void(boost::system::error_code const &)> handler) {
  socket_.reset(new boost::asio::ip::tcp::socket(service_));
  socket_->async_connect(endpoint, handler);
}

void network::http::normal_delegate::write(boost::asio::streambuf & command_streambuf,
                                           std::function<void(boost::system::error_code const &, size_t)> handler) {
  NETWORK_MESSAGE("normal_delegate::write(...)");
  NETWORK_MESSAGE("scheduling asynchronous write...");
  boost::asio::async_write(*socket_, command_streambuf, handler);
}

void network::http::normal_delegate::read_some(boost::asio::mutable_buffers_1 const & read_buffer,
                                               std::function<void(boost::system::error_code const &, size_t)> handler) {
  NETWORK_MESSAGE("normal_delegate::read_some(...)");
  NETWORK_MESSAGE("scheduling asynchronous read some...");
  socket_->async_read_some(read_buffer, handler);
  NETWORK_MESSAGE("scheduled asynchronous read some...");
}

network::http::normal_delegate::~normal_delegate() {}

#endif /* NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_NORMAL_DELEGATE_IPP_20110819 */
