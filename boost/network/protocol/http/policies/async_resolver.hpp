#ifndef BOOST_NETWORK_PROTOCOL_HTTP_POLICIES_ASYNC_RESOLVER_20100622
#define BOOST_NETWORK_PROTOCOL_HTTP_POLICIES_ASYNC_RESOLVER_20100622

// Copyright 2011 Dean Michael Berris <dberris@google.com>.
// Copyright 2011 Google, Inc. 
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <utility>
#include <boost/shared_ptr.hpp>
#include <boost/asio/ip/udp.hpp>

namespace boost { namespace network { namespace http { namespace policies {

struct async_resolver_pimpl;

struct async_resolver {
  typedef asio::ip::udp::resolver::iterator resolver_iterator;
  typedef std::pair<resolver_iterator, resolver_iterator>
          iterator_pair;
  typedef function<void(system::error_code const &, iterator_pair)>
          resolve_completion_function;

  explicit async_resolver(asio::io_service & service);
  void resolve(std::string const & host,
               uint16_t port,
               resolve_completion_function once_resolved);
  void clear_resolved_cache();
  ~async_resolver();

 protected:
  shared_ptr<async_resolver_pimpl> pimpl;
};

} // namespace policies

} // namespace http

} // namespace network

} // namespace boost

#ifdef BOOST_NETWORK_NO_LIB
#include <boost/network/protocol/http/policies/async_resolver.ipp>
#endif

#endif // BOOST_NETWORK_PROTOCOL_HTTP_POLICIES_ASYNC_RESOLVER_20100622
