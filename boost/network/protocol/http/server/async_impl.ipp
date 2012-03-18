#ifndef BOOST_NETWORK_PROTOCOL_HTTP_SERVER_ASYNC_IMPL_IPP_20120318
#define BOOST_NETWORK_PROTOCOL_HTTP_SERVER_ASYNC_IMPL_IPP_20120318

// Copyright 2012 Dean Michael Berris <dberris@google.com>.
// Copyright 2012 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/protocol/http/server/async_impl.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/bind.hpp>
#include <boost/network/detail/debug.hpp>

namespace boost { namespace network { namespace http {

async_server_impl::async_server_impl(server_options const &options,
                                     function<void(request const &, connection_ptr)> handler,
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
  lock_guard<mutex> listening_lock(listening_mutex_);
  if (listening_) {
    lock_guard<mutex> stopping_lock(stopping_mutex_);
    stopping_ = true;
    system::error_code ignored;
    acceptor_->close(ignored);
    listening_ = false;
    service_->post(
        boost::bind(&async_server_impl::handle_stop, this));
  }
}

void async_server_impl::listen() {
  lock_guard<mutex> listening_lock(listening_mutex_);
  BOOST_NETWORK_MESSAGE("listening on " << address_ << ':' << port_);
  if (!listening_) start_listening();
  if (!listening_) {
    BOOST_NETWORK_MESSAGE("error listening on " << address_ << ':' << port_);
    BOOST_THROW_EXCEPTION(std::runtime_error("Error listening on provided address:port."));
  }
}

void async_server_impl::handle_stop() {
  lock_guard<mutex> stopping_lock(stopping_mutex_);
  // A user may have stopped listening again before the stop command is
  // reached.
  if (stopping_) service_->stop();
}

void async_server_impl::handle_accept(boost::system::error_code const & ec) {
  {
    lock_guard<mutex> stopping_lock(stopping_mutex_);
    // We dont want to add another handler instance, and we dont want to know
    // about errors for a socket we dont need anymore.
    if (stopping_) return;
  }
  if (!ec) {
    set_socket_options(new_connection_->socket());
    new_connection_->start();
    new_connection_.reset(
        new async_server_connection(*service_, handler_, pool_));
    acceptor_->async_accept(
        new_connection_->socket(),
        boost::bind(
            &async_server_impl::handle_accept,
            this,
            asio::placeholders::error));
  } else {
    BOOST_NETWORK_MESSAGE("Error accepting connection, reason: " << ec);
  }
}

void async_server_impl::start_listening() {
  using asio::ip::tcp;
  system::error_code error;
  service_->reset();  // allows repeated cycles of run->stop->run
  tcp::resolver resolver(*service_);
  tcp::resolver::query query(address_, port_);
  tcp::resolver::iterator endpoint_iterator = resolver.resolve(query, error);
  if (error) {
    BOOST_NETWORK_MESSAGE("error resolving '" << address_ << ':' << port_);
    BOOST_THROW_EXCEPTION(std::runtime_error("Error resolving address:port combination."));
  }
  tcp::endpoint endpoint = *endpoint_iterator;
  acceptor_->open(endpoint.protocol(), error);
  if (error) {
    BOOST_NETWORK_MESSAGE("error opening socket: " << address_ << ":" << port_);
    BOOST_THROW_EXCEPTION(std::runtime_error("Error opening socket."));
  }
  set_acceptor_options(*acceptor_);
  acceptor_->bind(endpoint, error);
  if (error) {
    BOOST_NETWORK_MESSAGE("error binding socket: " << address_ << ":" << port_);
    BOOST_THROW_EXCEPTION(std::runtime_error("Error binding socket."));
  }
  acceptor_->listen(asio::socket_base::max_connections, error);
  if (error) {
    BOOST_NETWORK_MESSAGE("error listening on socket: '" << error << "' on " << address_ << ":" << port_);
    BOOST_THROW_EXCEPTION(std::runtime_error("Error listening on socket."));
  }
  new_connection_.reset(new async_server_connection(*service_, handler_, pool_));
  acceptor_->async_accept(
      new_connection_->socket(),
      boost::bind(
          &async_server_impl::handle_accept,
          this,
          asio::placeholders::error));
  listening_ = true;
  lock_guard<mutex> stopping_lock(stopping_mutex_);
  stopping_ = false; // if we were in the process of stopping, we revoke that command and continue listening
  BOOST_NETWORK_MESSAGE("now listening on '" << address_ << ":" << port_ << "'");
}

void async_server_impl::set_socket_options(asio::ip::tcp::socket &socket) {
  system::error_code ignored;
  socket.non_blocking(options_.non_blocking_io(), ignored);
  if (options_.linger()) {
    asio::ip::tcp::socket::linger linger(true, options_.linger_timeout());
    socket.set_option(linger, ignored);
  }
  if (int buf_size = options_.receive_buffer_size() >= 0) {
    asio::ip::tcp::socket::receive_buffer_size receive_buffer_size(buf_size);
    socket.set_option(receive_buffer_size, ignored);
  }
  if (int buf_size = options_.send_buffer_size() >= 0) {
    asio::ip::tcp::socket::send_buffer_size send_buffer_size(buf_size);
    socket.set_option(send_buffer_size, ignored);
  }
  if (int buf_size = options_.receive_low_watermark() >= 0) {
    asio::ip::tcp::socket::receive_low_watermark receive_low_watermark(buf_size);
    socket.set_option(receive_low_watermark, ignored);
  }
  if (int buf_size = options_.send_low_watermark() >= 0) {
    asio::ip::tcp::socket::send_low_watermark send_low_watermark(buf_size);
    socket.set_option(send_low_watermark, ignored);
  }
}

void async_server_impl::set_acceptor_options(asio::ip::tcp::acceptor &acceptor) {
  system::error_code ignored;
  acceptor.set_option(
      asio::ip::tcp::acceptor::reuse_address(options_.reuse_address()),
      ignored);
  acceptor.set_option(
      asio::ip::tcp::acceptor::enable_connection_aborted(options_.report_aborted()),
      ignored);
}

}  // namespace http

}  // namespace network

}  // namespace boost

#endif  // BOOST_NETWORK_PROTOCOL_HTTP_SERVER_ASYNC_IMPL_IPP_20120318
