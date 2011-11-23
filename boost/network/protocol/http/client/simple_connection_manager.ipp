#ifndef BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_SIMPLE_CONNECTION_MANAGER_IPP_20111112
#define BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_SIMPLE_CONNECTION_MANAGER_IPP_20111112

// Copyright 2011 Dean Michael Berris <dberris@google.com>.
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/protocol/http/client/simple_connection_manager.hpp>

namespace boost { namespace network { namespace http {

struct simple_connection_manager_pimpl {
  simple_connection_manager_pimpl(bool cache_resolved,
                                  bool follow_redirects,
                                  optional<std::string> openssl_certificate,
                                  optional<std::string> openssl_verify_path,
                                  shared_ptr<connection_factory> connection_factory)
  : cache_resolved_(cache_resolved_)
  , follow_redirects_(follow_redirects)
  , openssl_certificate_(openssl_certificate)
  , openssl_verify_path_(openssl_verify_path)
  , connection_factory_(connection_factory)
  {}

  simple_connection_manager_pimpl(bool cache_resolved,
                                  bool follow_redirects,
                                  std::string const & openssl_certificate,
                                  std::string const & openssl_verify_path,
                                  shared_ptr<connection_factory> connection_factory)
  : cache_resolved_(cache_resolved_)
  , follow_redirects_(follow_redirects)
  , openssl_certificate_(openssl_certificate)
  , openssl_verify_path_(openssl_verify_path)
  , connection_factory_(connection_factory)
  {}

  shared_ptr<client_connection> get_connection(asio::io_service & service,
                                               request_base const & request) {
    return connection_factory_->create_connection(
        service, request, cache_resolved_, follow_redirects_,
        openssl_certificate_, openssl_verify_path_);
  }

  void reset() {
    // do nothing here.
  }

  ~simple_connection_manager_pimpl() {
    // do nothing here.
  }

 private:
  bool cache_resolved_, follow_redirects_;
  optional<std::string> openssl_certificate_, openssl_verify_path_;
  shared_ptr<connection_factory> connection_factory_;

};

simple_connection_manager::simple_connection_manager(bool cache_resolved,
                                                     bool follow_redirects,
                                                     optional<std::string> openssl_certificate,
                                                     optional<std::string> openssl_verify_path,
                                                     shared_ptr<connection_factory> connection_factory)
: pimpl(new (std::nothrow) simple_connection_manager_pimpl(
    cache_resolved, follow_redirects, openssl_certificate, openssl_verify_path, connection_factory))
{}

simple_connection_manager::simple_connection_manager(bool cache_resolved,
                                                     bool follow_redirects,
                                                     std::string const & openssl_certificate,
                                                     std::string const & openssl_verify_path,
                                                     shared_ptr<connection_factory> connection_factory)
: pimpl(new (std::nothrow) simple_connection_manager_pimpl(
    cache_resolved, follow_redirects, openssl_certificate, openssl_verify_path, connection_factory))
{}

shared_ptr<client_connection> simple_connection_manager::get_connection(
    asio::io_service & service,
    request_base const & request) {
  return pimpl->get_connection(service, request);
}

void simple_connection_manager::reset() {
  pimpl->reset();
}

simple_connection_manager::~simple_connection_manager() {
  // do nothing here.
}

} /* http */

} /* network */

} /* boost */

#endif /* BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_SIMPLE_CONNECTION_MANAGER_IPP_20111112 */
