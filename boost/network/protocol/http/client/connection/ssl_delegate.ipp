#ifndef BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_SSL_DELEGATE_IPP_20110819
#define BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_SSL_DELEGATE_IPP_20110819

// Copyright 2011 Dean Michael Berris (dberris@google.com).
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/protocol/http/client/options.hpp>
#include <boost/network/protocol/http/client/connection/ssl_delegate.hpp>
#include <boost/bind.hpp>

boost::network::http::ssl_delegate::ssl_delegate(asio::io_service & service,
                                                 client_options const &options) :
  service_(service),
  options_(options) {}

void boost::network::http::ssl_delegate::connect(
    asio::ip::tcp::endpoint & endpoint,
    function<void(system::error_code const &)> handler) {
  context_.reset(new asio::ssl::context(
      service_,
      asio::ssl::context::sslv23_client));
  std::list<std::string> const & certificate_paths = 
      options_.openssl_certificate_paths();
  std::list<std::string> const & verifier_paths =
      options_.openssl_verify_paths();
  bool verify_peer = !certificate_paths.empty() || !verifier_paths.empty();
  if (verify_peer) context_->set_verify_mode(asio::ssl::context::verify_peer);
  else context_->set_verify_mode(asio::ssl::context::verify_none);
  for (std::list<std::string>::const_iterator it = certificate_paths.begin();
       it != certificate_paths.end(); ++it) {
    context_->load_verify_file(*it);
  }
  for (std::list<std::string>::const_iterator it = verifier_paths.begin();
       it != verifier_paths.begin(); ++it) {
    context_->add_verify_path(*it);
  }
  socket_.reset(new asio::ssl::stream<asio::ip::tcp::socket>(service_, *context_));
  socket_->lowest_layer().async_connect(
      endpoint,
      ::boost::bind(&boost::network::http::ssl_delegate::handle_connected,
           boost::network::http::ssl_delegate::shared_from_this(),
           asio::placeholders::error,
           handler));
}

void boost::network::http::ssl_delegate::handle_connected(system::error_code const & ec,
                                           function<void(system::error_code const &)> handler) {
  if (!ec) {
    socket_->async_handshake(asio::ssl::stream_base::client, handler);
  } else {
    handler(ec);
  }
}

void boost::network::http::ssl_delegate::write(
    asio::streambuf & command_streambuf,
    function<void(system::error_code const &, size_t)> handler) {
  asio::async_write(*socket_, command_streambuf, handler);
}

void boost::network::http::ssl_delegate::read_some(
    asio::mutable_buffers_1 const & read_buffer,
    function<void(system::error_code const &, size_t)> handler) {
  socket_->async_read_some(read_buffer, handler);
}

boost::network::http::ssl_delegate::~ssl_delegate() {}

#endif /* BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_SSL_DELEGATE_IPP_20110819 */
