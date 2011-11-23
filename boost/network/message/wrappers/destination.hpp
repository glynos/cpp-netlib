#ifndef __NETWORK_MESSAGE_WRAPPERS_DESTINATION_HPP__
#define __NETWORK_MESSAGE_WRAPPERS_DESTINATION_HPP__

// Copyright 2011 Dean Michael Berris <dberris@google.com>.
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/message/message_base.hpp>

namespace boost { namespace network {

namespace impl {

struct destination_wrapper {
  explicit destination_wrapper(message_base const & message);
  operator std::string () const;
 private:
  message_base const & message_;
  mutable optional<std::string> cache_;
};

} // namespace impl

inline std::string const
destination(message_base const & message_) {
  return impl::destination_wrapper(message_);
}

} // namespace network

} // namespace boost

#ifdef BOOST_NETWORK_NO_LIB
#include <boost/network/message/wrappers/destination.ipp>
#endif

#endif // __NETWORK_MESSAGE_WRAPPERS_DESTINATION_HPP__
