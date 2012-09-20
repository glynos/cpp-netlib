// Copyright 2010 (c) Dean Michael Berris
// Copyright 2012 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef NETWORK_MESSAGE_MODIFIER_REMOVE_HEADER_HPP_20100824
#define NETWORK_MESSAGE_MODIFIER_REMOVE_HEADER_HPP_20100824

#include <string>
#include <network/message/message_base.hpp>

namespace network {

inline
void remove_header(message_base & message, std::string const & key) {
   message.remove_headers(key);
}

} // namespace network

#endif // NETWORK_MESSAGE_MODIFIER_REMOVE_HEADER_HPP_20100824
