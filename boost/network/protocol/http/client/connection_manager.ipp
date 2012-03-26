#ifndef BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_MANAGER_IPP_20111103
#define BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_MANAGER_IPP_20111103

// Copyright 2011 Dean Michael Berris <dberris@google.com>.
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/protocol/http/client/connection_manager.hpp>
#include <boost/network/detail/debug.hpp>

namespace boost { namespace network { namespace http {

connection_manager::~connection_manager() {
  BOOST_NETWORK_MESSAGE("connection_manager::~connection_manager()");
  // default implementation, for linkage only.
}

} /* http */
} /* network */
} /* boost */

#endif /* BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_MANAGER_IPP_20111103 */
