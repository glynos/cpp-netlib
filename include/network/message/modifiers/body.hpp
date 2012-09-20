#ifndef NETWORK_MODIFIERS_BODY_HPP_20100824
#define NETWORK_MODIFIERS_BODY_HPP_20100824

// Copyright 2010 (c) Dean Michael Berris
// Copyright 2012 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

namespace network {

inline void body(message_base & message, std::string const & body_) {
  message.set_body(body_);
}

inline void append_body(message_base & message, std::string const & data) {
  message.append_body(data);
}

} // namespace network

#endif // NETWORK_MODIFIERS_BODY_HPP_20100824
