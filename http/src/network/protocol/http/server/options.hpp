// Copyright 2012 Dean Michael Berris <dberris@google.com>.
// Copyright 2012 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef NETWORK_PROTOCOL_HTTP_SERVER_OPTIONS_HPP_20120318
#define NETWORK_PROTOCOL_HTTP_SERVER_OPTIONS_HPP_20120318

#include <string>

namespace boost { namespace asio {

class io_service;

}  // namespace asio

}  // namespace boost

namespace network { namespace http {

class server_options_pimpl;

class server_options {
 public:
  server_options();
  server_options(server_options const &other);
  void swap(server_options &other);
  server_options& operator=(server_options rhs);
  ~server_options();
  
  server_options& address(std::string const &address);
  std::string const address() const;
  
  server_options& port(std::string const &port);
  std::string const port() const;
  
  server_options& io_service(boost::asio::io_service *service);
  boost::asio::io_service *io_service() const;

  server_options& reuse_address(bool setting);
  bool reuse_address() const;

  server_options& report_aborted(bool setting);
  bool report_aborted() const;

  // Set the receive buffer size for a socket. -1 means just use the default.
  server_options& receive_buffer_size(int buffer_size);
  int receive_buffer_size() const;

  // Set the send buffer size for a socket. -1 means just use the default.
  server_options& send_buffer_size(int buffer_size);
  int send_buffer_size() const;

  // Set the receive low watermark for a socket. -1 means just use the default.
  server_options& receive_low_watermark(int low_watermark);
  int receive_low_watermark() const;

  // Set the send low watermark for a socket. -1 means just use the default.
  server_options& send_low_watermark(int low_watermark);
  int send_low_watermark() const;

  server_options& non_blocking_io(bool setting);
  bool non_blocking_io() const;

  server_options& linger(bool setting);
  bool linger() const;

  // Set the socket linger timeout. This is only relevant if linger is true
  // (see linger above). -1 means just use the default.
  server_options& linger_timeout(int setting);
  int linger_timeout() const;

 private:
  server_options_pimpl *pimpl_;
};

}  // namespace http

}  // namespace network

#endif  // NETWORK_PROTOCOL_HTTP_SERVER_OPTIONS_HPP_20120318
