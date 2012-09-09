#ifndef BOOST_NETWORK_POLICY_ASYNC_CONNECTION_HPP_20100529
#define BOOST_NETWORK_POLICY_ASYNC_CONNECTION_HPP_20100529

// Copyright 2010 (C) Dean Michael Berris
// Copyright 2010 (C) Sinefunc, Inc.
// Copyright 2011 Dean Michael Berris (dberris@google.com).
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <network/version.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/cstdint.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <network/protocol/http/client/connection/resolver_delegate.hpp>
#include <boost/unordered_map.hpp>

namespace boost { namespace network { namespace http {

struct simple_async_connection_manager : connection_manager {
  simple_async_connection_manager(bool cache_resolved,
                                  bool follow_redirects,
                                  optional<std::string> openssl_certificate,
                                  optional<std::string> openssl_verify_path);
  simple_async_connection_manager(bool cache_resolved,
                                  bool follow_redirects,
                                  std::string const & openssl_certificate,
                                  std::string const & openssl_verify_path);
  virtual shared_ptr<client_connection> get_connection(
      asio::io_service & service,
      request_base const & request);  // override
  virtual void reset();  // override
  virtual ~simple_async_connection_manager();  // override
 protected:
  bool cache_resolved_, follow_redirects_;
  mutex shared_resolver_mutex;
  shared_ptr<resolver_delegate> shared_resolver_delegate;
};

struct http_1_1_async_connection;

struct http_1_1_async_connection_manager : connection_manager, enable_shared_from_this<http_1_1_async_connection_manager> {
  http_1_1_async_connection_manager(bool cache_resolved,
                                    bool follow_redirects,
                                    optional<std::string> openssl_certificate,
                                    optional<std::string> openssl_verify_path);
  http_1_1_async_connection_manager(bool cache_resolved,
                                    bool follow_redirects,
                                    std::string const & openssl_certificate,
                                    std::string const & openssl_verify_path);
  virtual shared_ptr<client_connection> get_connection(
      asio::io_service & service,
      request_base const & request);  // override
  virtual void reset();  // override
  virtual ~http_1_1_async_connection_manager();  // override

 protected:
  friend struct http_1_1_async_connection;
  void add_ready_connection(shared_ptr<client_connection> connection_ptr);
  shared_ptr<client_connection> get_ready_connection(std::string const & host);
  bool cache_resolved, follow_redirects_;
  mutex shared_resolver_mutex;
  shared_ptr<resolver_delegate> shared_resolver_delegate;
  boost::unordered_map<std::string, shared_ptr<client_connection> > ready_connections;
};

} // namespace http

} // namespace network

} // namespace boost

#endif // BOOST_NETWORK_POLICY_ASYNC_CONNECTION_HPP_
