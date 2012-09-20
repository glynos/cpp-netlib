// Copyright 2011 Dean Michael Berris (dberris@google.com).
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_RESOLVER_DELEGATE_FACTORY_HPP_20110930
#define NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_RESOLVER_DELEGATE_FACTORY_HPP_20110930

#include <boost/shared_ptr.hpp>
#include <network/protocol/http/request/request_base.hpp>
#include <network/protocol/http/client/connection/resolver_delegate.hpp>

namespace network { namespace http {

struct resolver_delegate_factory {
  resolver_delegate_factory();
  virtual shared_ptr<resolver_delegate> create_resolver_delegate(
      asio::io_service & service,
      bool cache_resolved);
  virtual ~resolver_delegate_factory();
 private:
  resolver_delegate_factory(resolver_delegate_factory const &);  // = delete
  resolver_delegate_factory& operator=(resolver_delegate_factory);  // = delete
};

}  // namespace http
}  // namespace network

#endif /* NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_RESOLVER_DELEGATE_FACTORY_HPP_20110930 */
