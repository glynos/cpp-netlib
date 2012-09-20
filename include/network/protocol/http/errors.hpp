// Copyright Dean Michael Berris 2007, 2008.
// Copyright 2012 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef NETWORK_PROTOCOL_HTTP_ERRORS_20080516_HPP
#define NETWORK_PROTOCOL_HTTP_ERRORS_20080516_HPP

#include <exception>

namespace network {
namespace http {
namespace errors {

struct connection_timeout_exception : std::runtime_error
{};

typedef connection_timeout_exception connection_timeout;

} // namespace errors
} // namespace http
} // namespace network

#endif // NETWORK_PROTOCOL_HTTP_20080516_HPP

