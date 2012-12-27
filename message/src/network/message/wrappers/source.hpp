// Copyright 2011 Dean Michael Berris <dberris@google.com>.
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef NETWORK_MESSAGE_WRAPPERS_SOURCE_HPP_20111021
#define NETWORK_MESSAGE_WRAPPERS_SOURCE_HPP_20111021

#include <network/message/message_base.hpp>

namespace network {

struct source_wrapper {
  explicit source_wrapper(message_base const & message);
  operator std::string () const;
 private:
  message_base const & message_;
  mutable boost::optional<std::string> cache_;
};

inline source_wrapper const
source(message_base const & message_) {
  return source_wrapper(message_);
}

inline std::ostream & operator<<(std::ostream &os, source_wrapper const &s) {
  return os << static_cast<std::string>(s);
}

} // namespace network

#endif // NETWORK_MESSAGE_WRAPPERS_SOURCE_HPP_20111021
