// Copyright 2010 (c) Dean Michael Berris
// Copyright 2010 (c) Sinefunc, Inc.
// Copyright 2012 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef NETWORK_PROTOCOL_HTTP_MESSAGE_MODIFIERS_VERSION_HPP_20100608
#define NETWORK_PROTOCOL_HTTP_MESSAGE_MODIFIERS_VERSION_HPP_20100608

#include <network/protocol/http/response/response_base.hpp>

namespace network {
namespace http {

inline void version(response_base & response, std::string const & value) {
  response.set_version(value);
}

} // namespace http
} // namespace network

#endif // NETWORK_PROTOCOL_HTTP_MESSAGE_MODIFIERS_VERSION_HPP_20100608
