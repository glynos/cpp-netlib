// Copyright 2011 Dean Michael Berris <dberris@google.com>.
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef NETWORK_MESSAGE_MODIFIER_DESTINATION_HPP_20100824
#define NETWORK_MESSAGE_MODIFIER_DESTINATION_HPP_20100824

namespace network {

inline void destination(message_base & message, std::string const & destination_) {
  message.set_destination(destination_);
}

} // namespace network

#endif // NETWORK_MESSAGE_MODIFIER_DESTINATION_HPP_20100824
