#ifndef BOOST_NETWORK_MESSAGE_MODIFIER_CLEAR_HEADERS_HPP_20100824
#define BOOST_NETWORK_MESSAGE_MODIFIER_CLEAR_HEADERS_HPP_20100824

// Copyright 2011 Dean Michael Berris <dberris@google.com>.
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <network/message/message_base.hpp>

namespace boost { namespace network {

inline void clear_headers(message_base & message) {
  message.remove_headers();
}

} // namespace network

} // namespace boost

#endif // BOOST_NETWORK_MESSAGE_MODIFIER_CLEAR_HEADERS_HPP_20100824
