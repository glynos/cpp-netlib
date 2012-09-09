#ifndef BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_IPP_20120306
#define BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_IPP_20120306

// Copyright 2012 Dean Michael Berris <dberris@google.com>.
// Copyright 2012 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/protocol/http/client.hpp>
#include <boost/network/protocol/http/client/options.hpp>
#include <boost/network/detail/debug.hpp>

namespace boost { namespace network { namespace http {

client::client()
: base_facade_type() {
  BOOST_NETWORK_MESSAGE("client::client()");
}

client::client(client_options const &options)
: base_facade_type(options)
{
  BOOST_NETWORK_MESSAGE("client::client(client_options const &)");
}

}  // namespace http
}  // namespace network
}  // namespace boost

#endif  // BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_IPP_20120306
