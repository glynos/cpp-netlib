// Copyright 2011 Dean Michael Berris <dberris@google.com>.
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_RESOLVER_DELEGATE_FACTORY_IPP_20111126
#define NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_RESOLVER_DELEGATE_FACTORY_IPP_20111126

#include <network/protocol/http/client/connection/resolver_delegate_factory.hpp>
#include <network/protocol/http/client/connection/async_resolver.hpp>
#include <network/detail/debug.hpp>

namespace network {
namespace http {

resolver_delegate_factory::resolver_delegate_factory() {
  NETWORK_MESSAGE("resolver_delegate_factory::resolver_delegate_factory()");
}

boost::shared_ptr<resolver_delegate>
resolver_delegate_factory::create_resolver_delegate(boost::asio::io_service & service,
                                                    bool cache_resolved) {
  NETWORK_MESSAGE("resolver_delegate_factory::create_resolver_delegate(...)");
  boost::shared_ptr<resolver_delegate> resolver_(
      new (std::nothrow) async_resolver(service, cache_resolved));
  return resolver_;
}

resolver_delegate_factory::~resolver_delegate_factory() {
  NETWORK_MESSAGE("resolver_delegate_factory::~resolver_delegate_factory()");
}

}  // namespace http
}  // namespace network

#endif  // NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_RESOLVER_DELEGATE_FACTORY_IPP_20111126
