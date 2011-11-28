#ifndef BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_CLIENT_CONNECTION_IPP_20111103
#define BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_CLIENT_CONNECTION_IPP_20111103

// Copyright 2011 Dean Michael Berris <dberris@google.com>.
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/protocol/http/client/client_connection.hpp>
#include <boost/assert.hpp>

namespace boost { namespace network { namespace http {

client_connection::~client_connection() {
  // For exposition only.
  BOOST_ASSERT(false && "This should not ever be called.");
}

client_connection * client_connection::clone() const {
  // For exposition only.
  BOOST_ASSERT(false && "This should not ever be called.");
}


} /* http */
  
} /* network */
  
} /* boost */

#endif /* BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_CLIENT_CONNECTION_IPP_20111103 */
