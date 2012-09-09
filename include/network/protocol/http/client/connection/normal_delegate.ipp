#ifndef BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_NORMAL_DELEGATE_IPP_20110819
#define BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_NORMAL_DELEGATE_IPP_20110819

// Copyright 2011 Dean Michael Berris (dberris@google.com).
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/asio/write.hpp>
#include <boost/function.hpp>
#include <boost/asio/buffer.hpp>
#include <network/protocol/http/client/connection/normal_delegate.hpp>
#include <network/detail/debug.hpp>

boost::network::http::normal_delegate::normal_delegate(asio::io_service & service)
  : service_(service)
{}

void boost::network::http::normal_delegate::connect(
    asio::ip::tcp::endpoint & endpoint,
    std::string const &host,
    function<void(system::error_code const &)> handler) {
  socket_.reset(new asio::ip::tcp::socket(service_));
  socket_->async_connect(endpoint, handler);
}

void boost::network::http::normal_delegate::write(
    asio::streambuf & command_streambuf,
    function<void(system::error_code const &, size_t)> handler) {
  BOOST_NETWORK_MESSAGE("normal_delegate::write(...)");
  BOOST_NETWORK_MESSAGE("scheduling asynchronous write...");
  asio::async_write(*socket_, command_streambuf, handler);
}

void boost::network::http::normal_delegate::read_some(
    asio::mutable_buffers_1 const & read_buffer,
    function<void(system::error_code const &, size_t)> handler) {
  BOOST_NETWORK_MESSAGE("normal_delegate::read_some(...)");
  BOOST_NETWORK_MESSAGE("scheduling asynchronous read some...");
  socket_->async_read_some(read_buffer, handler);
  BOOST_NETWORK_MESSAGE("scheduled asynchronous read some...");
}

boost::network::http::normal_delegate::~normal_delegate() {}

#endif /* BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_NORMAL_DELEGATE_IPP_20110819 */
