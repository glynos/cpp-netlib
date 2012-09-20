// Copyright 2011 Dean Michael Berris <dberris@google.com>.
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef NETWORK_PROTOCOL_HTTP_CLIENT_CLIENT_CONNECTION_IPP_20111103
#define NETWORK_PROTOCOL_HTTP_CLIENT_CLIENT_CONNECTION_IPP_20111103

#include <network/protocol/http/client/client_connection.hpp>
#include <boost/assert.hpp>
#include <network/detail/debug.hpp>

namespace network { namespace http {

client_connection::~client_connection() {
  NETWORK_MESSAGE("client_connection::~client_connection()");
  // Do nothing here.
}

client_connection * client_connection::clone() const {
  NETWORK_MESSAGE("client_connection::clone()");
  // For exposition only.
  ASSERT(false && "This should not ever be called.");
  return 0;
}


} /* http */
  
} /* network */
  
#endif /* NETWORK_PROTOCOL_HTTP_CLIENT_CLIENT_CONNECTION_IPP_20111103 */
