// Copyright 2012 Dean Michael Berris <dberris@google.com>.
// Copyright 2012 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef NETWORK_PROTOCOL_HTTP_SERVER_ASYNC_IMPL_IPP_20120318
#define NETWORK_PROTOCOL_HTTP_SERVER_ASYNC_IMPL_IPP_20120318

#include <network/protocol/http/server/async_impl.hpp>
#include <network/protocol/http/server/connection/async.hpp>
#include <asio/io_service.hpp>
#include <asio/ip/tcp.hpp>
#include <asio/placeholders.hpp>
#include <functional>
#include <network/detail/debug.hpp>

namespace network { namespace http {

async_server_impl::async_server_impl(server_options const &options,
                                     std::function<void(request const &, connection_ptr)> handler,
                                     utils::thread_pool &thread_pool)
: options_(options)
, address_(options.address())
, port_(options.port())
, service_(options.io_service())
, acceptor_(0)
, new_connection_()
, listening_mutex_()
, stopping_mutex_()
, handler_(handler)
, pool_(thread_pool)
, listening_(false)
, owned_service_(false)
, stopping_(false) {
  if (service_ == 0) {
    service_ = new (std::nothrow) asio::io_service;
    owned_service_ = true;
  }
  BOOST_ASSERT(service_ != 0);
  acceptor_ = new (std::nothrow) asio::ip::tcp::acceptor(*service_);
  BOOST_ASSERT(acceptor_ != 0);
}

async_server_impl::~async_server_impl() {
  if (owned_service_) delete service_;
  delete acceptor_;
}

void async_server_impl::run() {
  listen();
  service_->run();
}

void async_server_impl::stop() {
  boost::lock_guard<boost::mutex> listening_lock(listening_mutex_);
  if (listening_) {
    boost::lock_guard<boost::mutex> stopping_lock(stopping_mutex_);
    stopping_ = true;
    asio::error_code ignored;
    acceptor_->close(ignored);
    listening_ = false;
    service_->post(
        std::bind(&async_server_impl::handle_stop, this));
  }
}

void async_server_impl::listen() {
  boost::lock_guard<boost::mutex> listening_lock(listening_mutex_);
  NETWORK_MESSAGE("listening on " << address_ << ':' << port_);
  if (!listening_) start_listening();
  if (!listening_) {
    NETWORK_MESSAGE("error listening on " << address_ << ':' << port_);
    BOOST_THROW_EXCEPTION(std::runtime_error("Error listening on provided address:port."));
  }
}

void async_server_impl::handle_stop() {
  boost::lock_guard<boost::mutex> stopping_lock(stopping_mutex_);
  // A user may have stopped listening again before the stop command is
  // reached.
  if (stopping_) service_->stop();
}

void async_server_impl::handle_accept(asio::error_code const & ec) {
  {
    boost::lock_guard<boost::mutex> stopping_lock(stopping_mutex_);
    // We dont want to add another handler instance, and we dont want to know
    // about errors for a socket we dont need anymore.
    if (stopping_) return;
  }
  if (!ec) {
    set_socket_options(options_, new_connection_->socket());
    new_connection_->start();
    new_connection_.reset(
        new async_server_connection(*service_, handler_, pool_));
    acceptor_->async_accept(
        new_connection_->socket(),
        std::bind(
            &async_server_impl::handle_accept,
            this,
            asio::placeholders::error));
  } else {
    NETWORK_MESSAGE("Error accepting connection, reason: " << ec);
  }
}

void async_server_impl::start_listening() {
  using asio::ip::tcp;
  asio::error_code error;
  service_->reset();  // allows repeated cycles of run->stop->run
  tcp::resolver resolver(*service_);
  tcp::resolver::query query(address_, port_);
  tcp::resolver::iterator endpoint_iterator = resolver.resolve(query, error);
  if (error) {
    NETWORK_MESSAGE("error resolving '" << address_ << ':' << port_);
    BOOST_THROW_EXCEPTION(std::runtime_error("Error resolving address:port combination."));
  }
  tcp::endpoint endpoint = *endpoint_iterator;
  acceptor_->open(endpoint.protocol(), error);
  if (error) {
    NETWORK_MESSAGE("error opening socket: " << address_ << ":" << port_);
    BOOST_THROW_EXCEPTION(std::runtime_error("Error opening socket."));
  }
  set_acceptor_options(options_, *acceptor_);
  acceptor_->bind(endpoint, error);
  if (error) {
    NETWORK_MESSAGE("error binding socket: " << address_ << ":" << port_);
    BOOST_THROW_EXCEPTION(std::runtime_error("Error binding socket."));
  }
  acceptor_->listen(asio::socket_base::max_connections, error);
  if (error) {
    NETWORK_MESSAGE("error listening on socket: '" << error << "' on " << address_ << ":" << port_);
    BOOST_THROW_EXCEPTION(std::runtime_error("Error listening on socket."));
  }
  new_connection_.reset(new async_server_connection(*service_, handler_, pool_));
  acceptor_->async_accept(
      new_connection_->socket(),
      std::bind(
          &async_server_impl::handle_accept,
          this,
          asio::placeholders::error));
  listening_ = true;
  boost::lock_guard<boost::mutex> stopping_lock(stopping_mutex_);
  stopping_ = false; // if we were in the process of stopping, we revoke that command and continue listening
  NETWORK_MESSAGE("now listening on '" << address_ << ":" << port_ << "'");
}

}  // namespace http

}  // namespace network

#endif  // NETWORK_PROTOCOL_HTTP_SERVER_ASYNC_IMPL_IPP_20120318
