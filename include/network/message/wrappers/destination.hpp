// Copyright 2011 Dean Michael Berris <dberris@google.com>.
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef NETWORK_MESSAGE_WRAPPERS_DESTINATION_HPP
#define NETWORK_MESSAGE_WRAPPERS_DESTINATION_HPP

#include <network/message/message_base.hpp>

namespace network {

struct destination_wrapper {
  explicit destination_wrapper(message_base const & message);
  operator std::string () const;
 private:
  message_base const & message_;
  mutable optional<std::string> cache_;
};

inline destination_wrapper const
destination(message_base const & message_) {
  return destination_wrapper(message_);
}

inline std::ostream & operator<< (std::ostream &os, destination_wrapper const &d) {
  return os << static_cast<std::string>(d);
}

} // namespace network

#endif // NETWORK_MESSAGE_WRAPPERS_DESTINATION_HPP
