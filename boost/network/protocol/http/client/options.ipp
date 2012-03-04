// Copyright Dean Michael Berris 2012.
// Copyright Google, Inc. 2012.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_OPTIONS_IPP
#define BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_OPTIONS_IPP

#include <boost/network/protocol/http/client/options.hpp>

namespace boost { namespace network { namespace http {

class client_options_pimpl {
public:
  client_options_pimpl()
  : io_service_(0)
  , follow_redirects_(false)
  , cache_resolved_(false)
  , openssl_certificate_paths_()
  , openssl_verify_paths_()
  {}

  client_options_pimpl *clone() const {
    return new (std::nothrow) client_options_pimpl(*this);
  }

  void io_service(asio::io_service *io_service) {
    io_service_ = io_service_;
  }

  asio::io_service* io_service() const {
    return io_service_;
  }

  void follow_redirects(bool setting) {
    follow_redirects_ = setting;
  }

  bool follow_redirects() const {
    return follow_redirects_;
  }

  void cache_resolved(bool setting) {
    cache_resolved_ = setting;
  }

  bool cache_resolved() const {
    return cache_resolved_;
  }

  void add_openssl_certificate_path(std::string const &path) {
    openssl_certificate_paths_.push_back(path);
  }

  std::list<std::string> const & openssl_certificate_paths() const {
    return openssl_certificate_paths_;
  }

  void add_openssl_verify_path(std::string const &path) {
    openssl_verify_paths_.push_back(path);
  }

  std::list<std::string> const & openssl_verify_paths() const {
    return openssl_verify_paths_;
  }

private:
  client_options_pimpl(client_options_pimpl const &other)
  : io_service_(other.io_service_)
  , follow_redirects_(other.follow_redirects_)
  , cache_resolved_(other.cache_resolved_)
  , openssl_certificate_paths_(other.openssl_certificate_paths_)
  , openssl_verify_paths_(other.openssl_verify_paths_)
  {}

  client_options_pimpl& operator=(client_options_pimpl);  // cannot assign

  // Here's the list of members.
  asio::io_service *io_service_;
  bool follow_redirects_, cache_resolved_;
  std::list<std::string> openssl_certificate_paths_, openssl_verify_paths_;
};
  
client_options::client_options()
: pimpl(new (std::nothrow) client_options_pimpl())
{}

client_options::client_options(client_options const &other)
: pimpl(other.pimpl->clone())
{}

client_options& client_options::operator=(client_options rhs) {
  rhs.swap(*this);
  return *this;
}

void client_options::swap(client_options &other) {
  std::swap(other.pimpl, this->pimpl);
}

client_options::~client_options() {
  delete pimpl;
  pimpl = 0;
}

client_options& client_options::io_service(asio::io_service *io_service) {
  pimpl->io_service(io_service);
  return *this;
}

asio::io_service* client_options::io_service() const {
  return pimpl->io_service();
}

client_options& client_options::follow_redirects(bool follow_redirects) {
  pimpl->follow_redirects(follow_redirects);
  return *this;
}

bool client_options::follow_redirects() const {
  return pimpl->follow_redirects();
}

client_options& client_options::cache_resolved(bool cache_resolved) {
  pimpl->cache_resolved(cache_resolved);
  return *this;
}

bool client_options::cache_resolved() const {
  return pimpl->cache_resolved();
}

client_options& client_options::add_openssl_certificate_path(std::string const &path) {
  pimpl->add_openssl_certificate_path(path);
  return *this;
}

std::list<std::string> const & client_options::openssl_certificate_paths() const {
  return pimpl->openssl_certificate_paths();
}

client_options& client_options::add_openssl_verify_path(std::string const &path) {
  pimpl->add_openssl_verify_path(path);
  return *this;
}

std::list<std::string> const & client_options::openssl_verify_paths() const {
  return pimpl->openssl_verify_paths();
}

}  // namespace http
}  // namespace network
}  // namespace boost


#endif
