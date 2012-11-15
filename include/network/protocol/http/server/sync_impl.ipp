// Copyright 2012 Dean Michael Berris <dberris@google.com>.
// Copyright 2012 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef NETWORK_PROTOCOL_HTTP_SERVER_SYNC_IMPL_IPP_20120319
#define NETWORK_PROTOCOL_HTTP_SERVER_SYNC_IMPL_IPP_20120319

#include <boost/bind.hpp>
#include <network/protocol/http/server/sync_impl.hpp>
#include <network/protocol/http/server/connection/sync.hpp>
#include <network/detail/debug.hpp>

namespace network { namespace http {

sync_server_impl::sync_server_impl(server_options const &options,
                                   std::function<void(request const &, response &)> handler)
: options_(options)
, address_(options.address())
, port_(options.port())
, service_(options.io_service())
, acceptor_(0)
, new_connection_()
, listening_mutex_()
, listening_(false)
, owned_service_(false) {
  if (service_ == 0) {
    service_ = new boost::asio::io_service;
    owned_service_ = true;
    BOOST_ASSERT(service_ != 0);
  }
  acceptor_ = new boost::asio::ip::tcp::acceptor(*service_);
  BOOST_ASSERT(acceptor_ != 0);
}

void sync_server_impl::run() {
  listen();
  service_->run();
}

void sync_server_impl::stop() {
  boost::system::error_code ignored;
  acceptor_->close(ignored);
  service_->stop();
}

void sync_server_impl::listen() {
  std::lock_guard<std::mutex> listening_lock(listening_mutex_);
  if (!listening_) start_listening();
}

void sync_server_impl::handle_accept(boost::system::error_code const &ec) {
  if (!ec) {
    set_socket_options(options_, new_connection_->socket());
    new_connection_->start();
    new_connection_.reset(new sync_server_connection(*service_, handler_));
    acceptor_->async_accept(new_connection_->socket(),
        boost::bind(&sync_server_impl::handle_accept,
             this,
             boost::asio::placeholders::error));
  } else {
    NETWORK_MESSAGE("error accepting connection: " << ec);
    this->stop();
  }
}

void sync_server_impl::start_listening() {
  using boost::asio::ip::tcp;
  boost::system::error_code error;
  tcp::resolver resolver(*service_);
  tcp::resolver::query query(address_, port_);
  tcp::resolver::iterator endpoint_ = resolver.resolve(query, error);
  if (error) {
    NETWORK_MESSAGE("error resolving address: " << address_ << ':' << port_ << " -- reason: '" << error << '\'');
    BOOST_THROW_EXCEPTION(std::runtime_error("Error resolving provided address:port combination."));
  }
  tcp::endpoint endpoint = *endpoint_;
  acceptor_->open(endpoint.protocol(), error);
  if (error) {
    NETWORK_MESSAGE("error opening socket: " << address_ << ':' << port_ << " -- reason: '" << error << '\'');
    BOOST_THROW_EXCEPTION(std::runtime_error("Error opening socket for acceptor."));
  }
  set_acceptor_options(options_, *acceptor_);
  acceptor_->bind(endpoint, error);
  if (error) {
    NETWORK_MESSAGE("error boost::binding to socket: " << address_ << ':' << port_ << " -- reason: '" << error << '\'');
    BOOST_THROW_EXCEPTION(std::runtime_error("Error boost::binding to socket for acceptor."));
  }
  acceptor_->listen(tcp::socket::max_connections, error);
  if (error) {
    NETWORK_MESSAGE("error listening on socket: " << address_ << ':' << port_ << " -- reason: '" << error << '\'');
    BOOST_THROW_EXCEPTION(std::runtime_error("Error listening on socket for acceptor."));
  }
  new_connection_.reset(new sync_server_connection(*service_, handler_));
  acceptor_->async_accept(new_connection_->socket(),
                          boost::bind(&sync_server_impl::handle_accept,
                               this,
                               boost::asio::placeholders::error));
  listening_ = true;
}

}  // namespace http
}  // namespace network

#endif  // NETWORK_PROTOCOL_HTTP_SERVER_SYNC_IMPL_IPP_20120319
