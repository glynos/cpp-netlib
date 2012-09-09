#ifndef BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_CLIENT_CONNECTION_IPP_20111103
#define BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_CLIENT_CONNECTION_IPP_20111103

// Copyright 2011 Dean Michael Berris <dberris@google.com>.
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <network/protocol/http/client/client_connection.hpp>
#include <boost/assert.hpp>
#include <network/detail/debug.hpp>

namespace boost { namespace network { namespace http {

client_connection::~client_connection() {
  BOOST_NETWORK_MESSAGE("client_connection::~client_connection()");
  // Do nothing here.
}

client_connection * client_connection::clone() const {
  BOOST_NETWORK_MESSAGE("client_connection::clone()");
  // For exposition only.
  BOOST_ASSERT(false && "This should not ever be called.");
  return 0;
}


} /* http */
  
} /* network */
  
} /* boost */

#endif /* BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_CLIENT_CONNECTION_IPP_20111103 */
