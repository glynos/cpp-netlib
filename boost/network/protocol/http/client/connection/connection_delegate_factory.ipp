#ifndef BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_CONNECTION_DELEGATE_FACTORY_IPP_20111123
#define BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_CONNECTION_DELEGATE_FACTORY_IPP_20111123

// Copyright 2011 Dean Michael Berris <dberris@google.com>.
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/protocol/http/client/connection/normal_delegate.hpp>
#ifdef BOOST_NETWORK_ENABLE_HTTPS
#include <boost/network/protocol/http/client/connection/ssl_delegate.hpp>
#endif /* BOOST_NETWORK_ENABLE_HTTPS */

#include <boost/network/protocol/http/client/connection/connection_delegate_factory.hpp>
#include <boost/network/protocol/http/client/options.hpp>

namespace boost { namespace network { namespace http {

connection_delegate_factory::connection_delegate_factory() {}

connection_delegate_factory::connection_delegate_ptr
connection_delegate_factory::create_connection_delegate(
    asio::io_service & service,
    bool https,
    client_options const &options) {
  connection_delegate_ptr delegate;
  if (https) {
#ifdef BOOST_NETWORK_ENABLE_HTTPS
    delegate.reset(new ssl_delegate(service,
                                    options));
#else
    BOOST_THROW_EXCEPTION(std::runtime_error("HTTPS not supported."));
#endif /* BOOST_NETWORK_ENABLE_HTTPS */
  } else {
    delegate.reset(new normal_delegate(service));
  }
  return delegate;
}

connection_delegate_factory::~connection_delegate_factory() {}

} /* http */
  
} /* network */
  
} /* boost */

#endif /* BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_CONNECTION_DELEGATE_FACTORY_IPP_20111123 */
