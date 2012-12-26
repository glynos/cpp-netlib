// Copyright 2010 (c) Dean Michael Berris
// Copyright 2012 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef NETWORK_MESSAGE_MODIFIER_SOURCE_HPP_20100824
#define NETWORK_MESSAGE_MODIFIER_SOURCE_HPP_20100824

namespace network {

inline void source(message_base & message, std::string const & source_) {
  message.set_source(source_);
}

} // namespace network

#endif // NETWORK_MESSAGE_MODIFIER_SOURCE_HPP_20100824
