#ifndef __NETWORK_MESSAGE_WRAPPERS_SOURCE_HPP__
#define __NETWORK_MESSAGE_WRAPPERS_SOURCE_HPP__

// Copyright 2011 Dean Michael Berris <dberris@google.com>.
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/message/message_base.hpp>

namespace boost { namespace network {

namespace impl {

struct source_wrapper {
  explicit source_wrapper(message_base & message_);
  operator std::string () const;
 private:
  message_base & message_;
};

} // namespace impl

inline std::string const
source(message_base & message_) {
  return impl::source_wrapper(message_);
}

} // namespace network

} // namespace boost

#ifdef BOOST_NETWORK_NO_LIB
#include <boost/network/message/wrappers/source.ipp>
#endif

#endif // __NETWORK_MESSAGE_WRAPPERS_SOURCE_HPP__
