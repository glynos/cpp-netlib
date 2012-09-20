// Copyright 2012 Dean Michael Berris <dberris@google.com>.
// Copyright 2012 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef NETWORK_PROTOCOL_HTTP_CLIENT_IPP_20120306
#define NETWORK_PROTOCOL_HTTP_CLIENT_IPP_20120306

#include <network/protocol/http/client.hpp>
#include <network/protocol/http/client/options.hpp>
#include <network/detail/debug.hpp>

namespace network { namespace http {

client::client()
: base_facade_type() {
  NETWORK_MESSAGE("client::client()");
}

client::client(client_options const &options)
: base_facade_type(options)
{
  NETWORK_MESSAGE("client::client(client_options const &)");
}

}  // namespace http
}  // namespace network

#endif  // NETWORK_PROTOCOL_HTTP_CLIENT_IPP_20120306
