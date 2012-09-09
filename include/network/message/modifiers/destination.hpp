
#ifndef BOOST_NETWORK_MESSAGE_MODIFIER_DESTINATION_HPP_20100824
#define BOOST_NETWORK_MESSAGE_MODIFIER_DESTINATION_HPP_20100824

// Copyright 2011 Dean Michael Berris <dberris@google.com>.
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

namespace boost { namespace network {

inline void destination(message_base & message, std::string const & destination_) {
  message.set_destination(destination_);
}

} // namespace network

} // namespace boost

#endif // BOOST_NETWORK_MESSAGE_MODIFIER_DESTINATION_HPP_20100824
