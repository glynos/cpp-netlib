// Copyright 2011 Dean Michael Berris <dberris@google.com>.
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_MANAGER_IPP_20111103
#define NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_MANAGER_IPP_20111103

#include <network/protocol/http/client/connection_manager.hpp>
#include <network/detail/debug.hpp>

namespace network { namespace http {

connection_manager::~connection_manager() {
  NETWORK_MESSAGE("connection_manager::~connection_manager()");
  // default implementation, for linkage only.
}

}  // namespace http
}  // namespace network

#endif /* NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_MANAGER_IPP_20111103 */
