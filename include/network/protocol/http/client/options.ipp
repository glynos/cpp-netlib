// Copyright Dean Michael Berris 2012.
// Copyright Google, Inc. 2012.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef NETWORK_PROTOCOL_HTTP_CLIENT_OPTIONS_IPP
#define NETWORK_PROTOCOL_HTTP_CLIENT_OPTIONS_IPP

#include <network/protocol/http/client/options.hpp>
#include <network/protocol/http/client/simple_connection_manager.hpp>
#include <network/protocol/http/client/connection/simple_connection_factory.hpp>

namespace network { namespace http {

  class client_options_pimpl {
  public:
    client_options_pimpl()
    : io_service_(0)
    , follow_redirects_(false)
    , cache_resolved_(false)
    , openssl_certificate_paths_()
    , openssl_verify_paths_()
    , connection_manager_()
    , connection_factory_()
    {
    }

    client_options_pimpl *clone() const {
      return new (std::nothrow) client_options_pimpl(*this);
    }

    void io_service(boost::asio::io_service *io_service) {
      io_service_ = io_service_;
    }

    boost::asio::io_service* io_service() const {
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

    void connection_manager(std::shared_ptr<http::connection_manager> manager) {
      connection_manager_ = manager;
    }

    std::shared_ptr<http::connection_manager> connection_manager() const {
      return connection_manager_;
    }

    void connection_factory(std::shared_ptr<http::connection_factory> factory) {
      connection_factory_ = factory;
    }

    std::shared_ptr<http::connection_factory> connection_factory() const {
      return connection_factory_;
    }

  private:
    client_options_pimpl(client_options_pimpl const &other)
    : io_service_(other.io_service_)
    , follow_redirects_(other.follow_redirects_)
    , cache_resolved_(other.cache_resolved_)
    , openssl_certificate_paths_(other.openssl_certificate_paths_)
    , openssl_verify_paths_(other.openssl_verify_paths_)
    , connection_manager_(other.connection_manager_)
    , connection_factory_(other.connection_factory_)
    {}

    client_options_pimpl& operator=(client_options_pimpl);  // cannot assign

    // Here's the list of members.
    boost::asio::io_service *io_service_;
    bool follow_redirects_, cache_resolved_;
    std::list<std::string> openssl_certificate_paths_, openssl_verify_paths_;
    std::shared_ptr<http::connection_manager> connection_manager_;
    std::shared_ptr<http::connection_factory> connection_factory_;
  };

  client_options::client_options()
  : pimpl(new (std::nothrow) client_options_pimpl())
  {}

  client_options::client_options(client_options const &other)
  : pimpl(other.pimpl->clone())
  {}

  client_options& client_options::operator=(client_options const &rhs) {
    client_options copy(rhs);
    copy.swap(*this);
    return *this;
  }

  client_options& client_options::operator=(client_options &&rhs) {
    rhs.swap(*this);
    return *this;
  }

  void client_options::swap(client_options &other) {
    using std::swap;
    swap(other.pimpl, this->pimpl);
  }

  client_options::~client_options() {
    delete pimpl;
    pimpl = nullptr;
  }

  client_options& client_options::io_service(boost::asio::io_service *io_service) {
    pimpl->io_service(io_service);
    return *this;
  }

  boost::asio::io_service* client_options::io_service() const {
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

  client_options& client_options::connection_manager(std::shared_ptr<http::connection_manager> manager) {
    pimpl->connection_manager(manager);
    return *this;
  }

  std::shared_ptr<http::connection_manager> client_options::connection_manager() const {
    return pimpl->connection_manager();
  }

  client_options& client_options::connection_factory(std::shared_ptr<http::connection_factory> factory) {
    pimpl->connection_factory(factory);
    return *this;
  }

  std::shared_ptr<http::connection_factory> client_options::connection_factory() const {
    return pimpl->connection_factory();
  }

  // End of client_options.

  class request_options_pimpl {
  public:
    request_options_pimpl()
    : timeout_ms_(30 * 1000)
    , max_redirects_(10)
    {}

    request_options_pimpl *clone() const {
      return new (std::nothrow) request_options_pimpl(*this);
    }

    void timeout(uint64_t milliseconds) {
      timeout_ms_ = milliseconds;
    }

    uint64_t timeout() const {
      return timeout_ms_;
    }

    void max_redirects(int redirects) {
      max_redirects_ = redirects;
    }

    int max_redirects() const {
      return max_redirects_;
    }

  private:
    uint64_t timeout_ms_;
    int max_redirects_;

    request_options_pimpl(request_options_pimpl const &other)
    : timeout_ms_(other.timeout_ms_)
    , max_redirects_(other.max_redirects_)
    {}

    request_options_pimpl& operator=(request_options_pimpl);  // cannot be assigned.
  };

  request_options::request_options()
  : pimpl(new (std::nothrow) request_options_pimpl)
  {}

  request_options::request_options(request_options const &other)
  : pimpl(other.pimpl->clone())
  {}

  request_options& request_options::operator=(request_options rhs) {
    rhs.swap(*this);
    return *this;
  }

  void request_options::swap(request_options &other) {
    std::swap(other.pimpl, this->pimpl);
  }
  
  request_options::~request_options() {
    delete pimpl;
  }
  
  request_options& request_options::timeout(uint64_t milliseconds) {
    pimpl->timeout(milliseconds);
    return *this;
  }
  
  uint64_t request_options::timeout() const {
    return pimpl->timeout();
  }
  
  request_options& request_options::max_redirects(int redirects) {
    pimpl->max_redirects(redirects);
    return *this;
  }
  
  int request_options::max_redirects() const {
    return pimpl->max_redirects();
  }
  
}  // namespace http
}  // namespace network

#endif
