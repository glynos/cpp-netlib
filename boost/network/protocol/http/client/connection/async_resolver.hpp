#ifndef BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_ASYNC_RESOLVER_20111126
#define BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_ASYNC_RESOLVER_20111126

// Copyright 2011 Dean Michael Berris <dberris@google.com>.
// Copyright 2011 Google, Inc. 
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/shared_ptr.hpp>
#include <boost/network/protocol/http/client/connection/resolver_delegate.hpp>

namespace boost { namespace network { namespace http {

struct async_resolver_pimpl;

struct async_resolver : resolver_delegate {
  using resolver_delegate::resolve_completion_function;

  async_resolver(asio::io_service & service, bool cache_resolved);
  virtual void resolve(std::string const & host,
                       uint16_t port,
                       resolve_completion_function once_resolved);  // override
  virtual void clear_resolved_cache();  // override
  virtual ~async_resolver();

 protected:
  // We need a shared_ptr because the pimpl may live on long after the resolver
  // delegate (instances of this type) is actually destroyed.
  shared_ptr<async_resolver_pimpl> pimpl;
};

} // namespace http

} // namespace network

} // namespace boost

#ifdef BOOST_NETWORK_NO_LIB
#include <boost/network/protocol/http/policies/async_resolver.ipp>
#endif

#endif // BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_ASYNC_RESOLVER_20111126
