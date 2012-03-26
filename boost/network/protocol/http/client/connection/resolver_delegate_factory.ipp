#ifndef BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_RESOLVER_DELEGATE_FACTORY_IPP_20111126
#define BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_RESOLVER_DELEGATE_FACTORY_IPP_20111126

// Copyright 2011 Dean Michael Berris <dberris@google.com>.
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/protocol/http/client/connection/resolver_delegate_factory.hpp>
#include <boost/network/protocol/http/client/connection/async_resolver.hpp>
#include <boost/network/detail/debug.hpp>

namespace boost { namespace network { namespace http {

resolver_delegate_factory::resolver_delegate_factory() {
  BOOST_NETWORK_MESSAGE("resolver_delegate_factory::resolver_delegate_factory()");
}

shared_ptr<resolver_delegate>
resolver_delegate_factory::create_resolver_delegate(asio::io_service & service,
                                                    bool cache_resolved) {
  BOOST_NETWORK_MESSAGE("resolver_delegate_factory::create_resolver_delegate(...)");
  shared_ptr<resolver_delegate> resolver_(
      new (std::nothrow) async_resolver(service, cache_resolved));
  return resolver_;
}

resolver_delegate_factory::~resolver_delegate_factory() {
  BOOST_NETWORK_MESSAGE("resolver_delegate_factory::~resolver_delegate_factory()");
}

}  // namespace http

}  // namespace network

}  // namespace boost

#endif  // BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_RESOLVER_DELEGATE_FACTORY_IPP_20111126
