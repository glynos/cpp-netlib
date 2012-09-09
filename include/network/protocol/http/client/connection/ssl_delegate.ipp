#ifndef BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_SSL_DELEGATE_IPP_20110819
#define BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_SSL_DELEGATE_IPP_20110819

// Copyright 2011 Dean Michael Berris (dberris@google.com).
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <network/protocol/http/client/options.hpp>
#include <network/protocol/http/client/connection/ssl_delegate.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/bind.hpp>
#include <network/detail/debug.hpp>

boost::network::http::ssl_delegate::ssl_delegate(asio::io_service & service,
                                                 client_options const &options) :
  service_(service),
  options_(options) {
  BOOST_NETWORK_MESSAGE("ssl_delegate::ssl_delegate(...)");
}

void boost::network::http::ssl_delegate::connect(
    asio::ip::tcp::endpoint & endpoint,
    std::string const &host,
    function<void(system::error_code const &)> handler) {
  BOOST_NETWORK_MESSAGE("ssl_delegate::connect(...)");
  context_.reset(new asio::ssl::context(
      asio::ssl::context::sslv23));
  std::list<std::string> const & certificate_paths = 
      options_.openssl_certificate_paths();
  std::list<std::string> const & verifier_paths =
      options_.openssl_verify_paths();
  bool use_default_verification = certificate_paths.empty() && verifier_paths.empty();
  if (!use_default_verification) {
    for (std::list<std::string>::const_iterator it = certificate_paths.begin();
         it != certificate_paths.end(); ++it) {
      context_->load_verify_file(*it);
    }
    for (std::list<std::string>::const_iterator it = verifier_paths.begin();
         it != verifier_paths.begin(); ++it) {
      context_->add_verify_path(*it);
    }
    BOOST_NETWORK_MESSAGE("verifying peer: " << host);
    context_->set_verify_mode(asio::ssl::context::verify_peer);
    context_->set_verify_callback(asio::ssl::rfc2818_verification(host));
  } else {
    BOOST_NETWORK_MESSAGE("not verifying peer");
    context_->set_default_verify_paths();
    context_->set_verify_mode(asio::ssl::context::verify_peer);
    context_->set_verify_callback(asio::ssl::rfc2818_verification(host));
  }
  socket_.reset(new asio::ssl::stream<asio::ip::tcp::socket>(service_, *context_));
  BOOST_NETWORK_MESSAGE("scheduling asynchronous connection...");
  socket_->lowest_layer().async_connect(
      endpoint,
      ::boost::bind(&boost::network::http::ssl_delegate::handle_connected,
           boost::network::http::ssl_delegate::shared_from_this(),
           asio::placeholders::error,
           handler));
}


void boost::network::http::ssl_delegate::handle_connected(
    system::error_code const & ec,
    function<void(system::error_code const &)> handler) {
  BOOST_NETWORK_MESSAGE("ssl_delegate::handle_connected(...)");
  if (!ec) {
    BOOST_NETWORK_MESSAGE("connected to endpoint.");
    // Here we check if there's an existing session for the connection.
    SSL_SESSION *existing_session = SSL_get1_session(socket_->impl()->ssl);
    if (existing_session == NULL) {
      BOOST_NETWORK_MESSAGE("found no existing session, performing handshake.");
      socket_->async_handshake(asio::ssl::stream_base::client, handler);
    } else {
      BOOST_NETWORK_MESSAGE("found existing session, bypassing handshake.");
      SSL_set_session(socket_->impl()->ssl, existing_session);
      SSL_connect(socket_->impl()->ssl);
      handler(ec);
    }
  } else {
    BOOST_NETWORK_MESSAGE("encountered error: " << ec);
    handler(ec);
  }
}

void boost::network::http::ssl_delegate::write(
    asio::streambuf & command_streambuf,
    function<void(system::error_code const &, size_t)> handler) {
  BOOST_NETWORK_MESSAGE("ssl_delegate::write(...)");
  BOOST_NETWORK_MESSAGE("scheduling asynchronous write...");
  asio::async_write(*socket_, command_streambuf, handler);
}

void boost::network::http::ssl_delegate::read_some(
    asio::mutable_buffers_1 const & read_buffer,
    function<void(system::error_code const &, size_t)> handler) {
  BOOST_NETWORK_MESSAGE("ssl_delegate::read_some(...)");
  BOOST_NETWORK_MESSAGE("scheduling asynchronous read_some...");
  socket_->async_read_some(read_buffer, handler);
}

boost::network::http::ssl_delegate::~ssl_delegate() {
  BOOST_NETWORK_MESSAGE("ssl_delegate::~ssl_delegate()");
}

#endif /* BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_SSL_DELEGATE_IPP_20110819 */
