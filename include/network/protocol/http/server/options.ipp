// Copyright 2012 Dean Michael Berris <dberris@google.com>.
// Copyright 2012 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_NETWORK_PROTOCOL_HTTP_SERVER_OPTIONS_IPP_20120318
#define BOOST_NETWORK_PROTOCOL_HTTP_SERVER_OPTIONS_IPP_20120318

#include <network/protocol/http/server/options.hpp>
#include <boost/asio/io_service.hpp>

namespace network { namespace http {

class server_options_pimpl {
 public:
  server_options_pimpl()
  : address_("0.0.0.0")
  , port_("80")
  , io_service_(0)
  , receive_buffer_size_(-1)
  , send_buffer_size_(-1)
  , receive_low_watermark_(-1)
  , send_low_watermark_(-1)
  , linger_timeout_(30)
  , reuse_address_(false)
  , report_aborted_(false)
  , non_blocking_io_(true)
  , linger_(false)
  {}

  server_options_pimpl *clone() const {
    return new server_options_pimpl(*this);
  }

  void address(std::string const &address) {
    address_ = address;
  }

  std::string const address() const {
    return address_;
  }

  void port(std::string const &port) {
    port_ = port;
  }

  std::string const port() const {
    return port_;
  }

  void io_service(asio::io_service *service) {
    io_service_ = service;
  }

  asio::io_service *io_service() const {
    return io_service_;
  }

  void reuse_address(bool setting) {
    reuse_address_ = setting;
  }

  bool reuse_address() const {
    return reuse_address_;
  }

  void report_aborted(bool setting) {
    report_aborted_ = setting;
  }

  bool report_aborted() const {
    return report_aborted_;
  }

  void receive_buffer_size(int buffer_size) {
    receive_buffer_size_ = buffer_size;
  }

  int receive_buffer_size() const {
    return receive_buffer_size_;
  }

  void send_buffer_size(int buffer_size) {
    send_buffer_size_ = buffer_size;
  }

  int send_buffer_size() const {
    return send_buffer_size_;
  }

  void receive_low_watermark(int buffer_size) {
    receive_low_watermark_ = buffer_size;
  }

  int receive_low_watermark() const {
    return receive_low_watermark_;
  }

  void send_low_watermark(int buffer_size) {
    send_low_watermark_ = buffer_size;
  }

  int send_low_watermark() const {
    return send_low_watermark_;
  }

  void non_blocking_io(bool setting) {
    non_blocking_io_ = setting;
  }

  bool non_blocking_io() const {
    return non_blocking_io_;
  }

  void linger(bool setting) {
    linger_ = setting;
  }

  bool linger() const {
    return linger_;
  }

  void linger_timeout(int setting) {
    linger_timeout_  = setting;
  }

  int linger_timeout() const {
    return linger_timeout_;
  }

 private:
  std::string address_, port_;
  asio::io_service *io_service_;
  int receive_buffer_size_, send_buffer_size_,
      receive_low_watermark_, send_low_watermark_,
      linger_timeout_;
  bool reuse_address_, report_aborted_,
       non_blocking_io_, linger_;

  server_options_pimpl(server_options_pimpl const &other)
  : address_(other.address_)
  , port_(other.port_)
  , io_service_(other.io_service_)
  , receive_buffer_size_(other.receive_buffer_size_)
  , send_buffer_size_(other.send_buffer_size_)
  , receive_low_watermark_(other.receive_low_watermark_)
  , send_low_watermark_(other.send_low_watermark_)
  , linger_timeout_(other.linger_timeout_)
  , reuse_address_(other.reuse_address_)
  , report_aborted_(other.report_aborted_)
  , non_blocking_io_(other.non_blocking_io_)
  , linger_(other.linger_) {}

};

server_options::server_options()
: pimpl_(new (std::nothrow) server_options_pimpl)
{}

server_options::server_options(server_options const &other)
: pimpl_(other.pimpl_->clone())
{}

server_options::~server_options() {
  delete pimpl_;
}

void server_options::swap(server_options &other) {
  std::swap(other.pimpl_, this->pimpl_);
}

server_options& server_options::operator=(server_options other) {
  other.swap(*this);
  return *this;
}

server_options& server_options::address(std::string const &address) {
  pimpl_->address(address);
  return *this;
}

std::string const server_options::address() const {
  return pimpl_->address();
}

server_options& server_options::port(std::string const &port) {
  pimpl_->port(port);
  return *this;
}

std::string const server_options::port() const {
  return pimpl_->port();
}

server_options& server_options::io_service(asio::io_service *io_service) {
  pimpl_->io_service(io_service);
  return *this;
}

asio::io_service* server_options::io_service() const {
  return pimpl_->io_service();
}

server_options& server_options::non_blocking_io(bool setting) {
  pimpl_->non_blocking_io(setting);
  return *this;
}

bool server_options::non_blocking_io() const {
  return pimpl_->non_blocking_io();
}

server_options& server_options::reuse_address(bool setting) {
  pimpl_->reuse_address(setting);
  return *this;
}

bool server_options::reuse_address() const {
  return pimpl_->reuse_address();
}

server_options& server_options::report_aborted(bool setting) {
  pimpl_->report_aborted(setting);
  return *this;
}

bool server_options::report_aborted() const {
  return pimpl_->report_aborted();
}

server_options& server_options::receive_buffer_size(int buffer_size) {
  pimpl_->receive_buffer_size(buffer_size);
  return *this;
}

int server_options::receive_buffer_size() const {
  return pimpl_->receive_buffer_size();
}

server_options& server_options::send_buffer_size(int buffer_size) {
  pimpl_->send_buffer_size(buffer_size);
  return *this;
}

int server_options::send_buffer_size() const {
  return pimpl_->send_buffer_size();
}

server_options& server_options::receive_low_watermark(int buffer_size) {
  pimpl_->receive_low_watermark(buffer_size);
  return *this;
}

int server_options::receive_low_watermark() const {
  return pimpl_->receive_low_watermark();
}

server_options& server_options::send_low_watermark(int buffer_size) {
  pimpl_->send_low_watermark(buffer_size);
  return *this;
}

int server_options::send_low_watermark() const {
  return pimpl_->send_low_watermark();
}

server_options& server_options::linger(bool setting) {
  pimpl_->linger(setting);
  return *this;
}

bool server_options::linger() const {
  return pimpl_->linger();
}

server_options& server_options::linger_timeout(int buffer_size) {
  pimpl_->linger_timeout(buffer_size);
  return *this;
}

int server_options::linger_timeout() const {
  return pimpl_->linger_timeout();
}

}  // namespace http

}  // namespace network

#endif  // BOOST_NETWORK_PROTOCOL_HTTP_SERVER_OPTIONS_IPP_20120318
