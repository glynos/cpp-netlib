#ifndef NETWORK_MESSAGE_MODIFIER_ADD_HEADER_HPP_20100824
#define NETWORK_MESSAGE_MODIFIER_ADD_HEADER_HPP_20100824

// Copyright 2011 Dean Michael Berris <dberris@google.com>.
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/utility/enable_if.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/not.hpp>

namespace network {

inline void add_header(message_base & message,
                       std::string const & key,
                       std::string const & value) {
  message.append_header(key, value);
}

} // namespace network

#endif // NETWORK_MESSAGE_MODIFIER_ADD_HEADER_HPP_20100824
