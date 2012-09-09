#ifndef BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_CONNECTION_DELEGATE_FACTORY_IPP_20111123
#define BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_CONNECTION_DELEGATE_FACTORY_IPP_20111123

// Copyright 2011 Dean Michael Berris <dberris@google.com>.
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <network/protocol/http/client/connection/normal_delegate.hpp>
#ifdef BOOST_NETWORK_ENABLE_HTTPS
#include <network/protocol/http/client/connection/ssl_delegate.hpp>
#endif /* BOOST_NETWORK_ENABLE_HTTPS */

#include <network/protocol/http/client/connection/connection_delegate_factory.hpp>
#include <network/protocol/http/client/options.hpp>
#include <network/detail/debug.hpp>

namespace boost { namespace network { namespace http {

connection_delegate_factory::connection_delegate_factory() {
  BOOST_NETWORK_MESSAGE("connection_delegate_factory::connection_delegate_factory()");
}

connection_delegate_factory::connection_delegate_ptr
connection_delegate_factory::create_connection_delegate(
    asio::io_service & service,
    bool https,
    client_options const &options) {
  BOOST_NETWORK_MESSAGE("connection_delegate_factory::create_connection_delegate(...)");
  connection_delegate_ptr delegate;
  if (https) {
#ifdef BOOST_NETWORK_ENABLE_HTTPS
    BOOST_NETWORK_MESSAGE("creating an SSL delegate");
    delegate.reset(new ssl_delegate(service,
                                    options));
#else
    BOOST_NETWORK_MESSAGE("creating an SSL delegate, but not supported");
    BOOST_THROW_EXCEPTION(std::runtime_error("HTTPS not supported."));
#endif /* BOOST_NETWORK_ENABLE_HTTPS */
  } else {
    BOOST_NETWORK_MESSAGE("creating a normal delegate");
    delegate.reset(new normal_delegate(service));
  }
  return delegate;
}

connection_delegate_factory::~connection_delegate_factory() {
  BOOST_NETWORK_MESSAGE("connection_delegate_factory::~connection_delegate_factory()");
}

} /* http */
  
} /* network */
  
} /* boost */

#endif /* BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_CONNECTION_DELEGATE_FACTORY_IPP_20111123 */
