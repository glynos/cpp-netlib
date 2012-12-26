// Copyright Dean Michael Berris 2007.
// Copyright 2012 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef NETWORK_MESSAGE_WRAPPERS_HEADERS_HPP__
#define NETWORK_MESSAGE_WRAPPERS_HEADERS_HPP__

#include <map>

namespace network {

struct message_base;

struct headers_wrapper {
  typedef std::multimap<std::string, std::string> container_type;
  explicit headers_wrapper(message_base const & message);
  operator container_type () const;
 private:
  message_base const & message_;
};

/// Factory method to create the right wrapper object
inline headers_wrapper const
headers(message_base const & message_) {
    return headers_wrapper(message_);
}

} // namespace network

#endif // __NETWORK_MESSAGE_WRAPPERS_HEADERS_HPP__
