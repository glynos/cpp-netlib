// Copyright 2011 Dean Michael Berris <dberris@google.com>.
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_HPP_20111016
#define NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_HPP_20111016

#include <utility>
#include <boost/asio/ip/udp.hpp>
#include <boost/function.hpp>

namespace network {
namespace http {

struct resolver_delegate {
  typedef asio::ip::udp::resolver::iterator resolver_iterator;
  typedef std::pair<resolver_iterator, resolver_iterator>
          iterator_pair;
  typedef function<void(system::error_code const &, iterator_pair)>
          resolve_completion_function;
  virtual void resolve(std::string const & host,
                       uint16_t port,
                       resolve_completion_function once_resolved) = 0;
  virtual void clear_resolved_cache() = 0;
  virtual ~resolver_delegate() = 0;
};
  
}  // namespace http
}  // namespace network

#endif /* NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_HPP_20111016 */
