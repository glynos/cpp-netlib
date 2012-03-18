#ifndef BOOST_NETWORK_PROTOCOL_HTTP_SERVER_OPTIONS_HPP_20120318
#define BOOST_NETWORK_PROTOCOL_HTTP_SERVER_OPTIONS_HPP_20120318

// Copyright 2012 Dean Michael Berris <dberris@google.com>.
// Copyright 2012 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <string>

namespace boost { namespace asio {

class io_service;

}  // namespace asio

}  // namespace boost

namespace boost { namespace network { namespace http {

class server_options_pimpl;

class server_options {
 public:
  server_options();
  server_options(server_options const &other);
  void swap(server_options &other);
  server_options& operator=(server_options rhs);
  
  server_options& address(std::string const &address="0.0.0.0");
  std::string const address() const;
  
  server_options& port(std::string const &port="80");
  std::string const port() const;
  
  server_options& io_service(asio::io_service *service = 0);
  asio::io_service *io_service() const;

  server_options& reuse_address(bool setting=true);
  bool reuse_address();

  server_options& report_aborted(bool setting=false);
  bool report_aborted();

  // Set the receive buffer size for a socket. -1 means just use the default.
  server_options& receive_buffer_size(int buffer_size=-1);
  int receive_buffer_size();

  // Set the send buffer size for a socket. -1 means just use the default.
  server_options& send_buffer_size(int buffer_size=-1);
  int send_buffer_size();

  // Set the receive low watermark for a socket. -1 means just use the default.
  server_options& receive_low_watermark(int low_watermark=-1);
  int receive_low_watermark();

  // Set the send low watermark for a socket. -1 means just use the default.
  server_options& send_low_watermark(int low_watermark=-1);
  int send_low_watermark();

  server_options& non_blocking_io(bool setting=true);
  bool non_blocking_io();

  server_options& linger(bool setting=false);
  bool linger();

  // Set the socket linger timeout. This is only relevant if linger is true
  // (see linger above). -1 means just use the default.
  server_options& linger_timeout(int setting=-1);
  int linger_timeout();

 private:
  server_options_pimpl *pimpl_;
};

}  // namespace http

}  // namespace network

}  // namespace boost

#endif  // BOOST_NETWORK_PROTOCOL_HTTP_SERVER_OPTIONS_HPP_20120318
