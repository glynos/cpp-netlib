#ifndef BOOST_NETWORK_POLICY_ASYNC_CONNECTION_HPP_20100529
#define BOOST_NETWORK_POLICY_ASYNC_CONNECTION_HPP_20100529

// Copyright 2010 (C) Dean Michael Berris
// Copyright 2010 (C) Sinefunc, Inc.
// Copyright 2011 Dean Michael Berris (dberris@google.com).
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/version.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/cstdint.hpp>
#include <boost/network/protocol/http/traits/resolver_policy.hpp>
#include <boost/network/protocol/http/client/connection/async_base.hpp>
#include <boost/network/protocol/http/client/connection_manager.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/algorithm/string/predicate.hpp>

namespace boost { namespace network { namespace http {

struct simple_async_connection_manager : connection_manager {
  simple_async_connection_manager(bool cache_resolved,
                                  bool follow_redirects,
                                  optional<std::string> openssl_certificate,
                                  optional<std::string> openssl_verify_path);
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

struct http_1_1_async_connection_manager_pimpl;

struct http_1_1_async_connection_manager : connection_manager {
  http_1_1_async_connection_manager(bool cache_resolved,
                                    bool follow_redirects,
                                    optional<std::string> openssl_certificate,
                                    optional<std::string> openssl_verify_path);
  virtual shared_ptr<client_connection> get_connection(
      asio::io_service & service,
      request_base const & request);  // override
  virtual void reset();  // override
  virtual ~http_1_1_async_connection_manager();  // override
};

template <class Tag, unsigned version_major, unsigned version_minor>
struct async_connection_policy;

template <class Tag>
struct async_connection_policy<Tag,1u,0u> {  // HTTP/1.0
  typedef simple_async_connection_manager type;
}

template <class Tag>
struct async_connection_policy<Tag,1u,1u> {  // HTTP/1.1
  typedef http_1_1_connection_manager type;
}

} // namespace http

} // namespace network

} // namespace boost

#endif // BOOST_NETWORK_POLICY_ASYNC_CONNECTION_HPP_
