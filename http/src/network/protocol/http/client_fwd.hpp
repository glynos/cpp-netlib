// Copyright Dean Michael Berris 2007-2008.
// Copyright 2012 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef NETWORK_PROTOCOL_HTTP_CLIENT_20080923_1_HPP
#define NETWORK_PROTOCOL_HTTP_CLIENT_20080923_1_HPP

#include <network/version.hpp>
#include <string>

namespace network {
namespace http {

// Forward declaration of basic_client template.
template <class String>
class basic_client;

typedef basic_client<std::string> client;

} // namespace http
} // namespace network

#endif // NETWORK_PROTOCOL_HTTP_CLIENT_20080923_1_HPP
