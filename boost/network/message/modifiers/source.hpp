
#ifndef BOOST_NETWORK_MESSAGE_MODIFIER_SOURCE_HPP_20100824
#define BOOST_NETWORK_MESSAGE_MODIFIER_SOURCE_HPP_20100824

// Copyright 2010 (c) Dean Michael Berris
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

namespace boost { namespace network {

inline void source(message_base & message, std::string const & source_) {
  message.set_source(source_);
}

} // namespace network

} // namespace boost

#endif // BOOST_NETWORK_MESSAGE_MODIFIER_SOURCE_HPP_20100824
