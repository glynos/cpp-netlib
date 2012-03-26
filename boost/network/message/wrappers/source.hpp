#ifndef BOOST_NETWORK_MESSAGE_WRAPPERS_SOURCE_HPP_20111021
#define BOOST_NETWORK_MESSAGE_WRAPPERS_SOURCE_HPP_20111021

// Copyright 2011 Dean Michael Berris <dberris@google.com>.
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/message/message_base.hpp>

namespace boost { namespace network {

struct source_wrapper {
  explicit source_wrapper(message_base & message);
  operator std::string () const;
 private:
  message_base & message_;
  mutable boost::optional<std::string> cache_;
};

inline source_wrapper const
source(message_base & message_) {
  return source_wrapper(message_);
}

inline std::ostream & operator<<(std::ostream &os, source_wrapper const &s) {
  return os << static_cast<std::string>(s);
}

} // namespace network

} // namespace boost

#ifdef BOOST_NETWORK_NO_LIB
#include <boost/network/message/wrappers/source.ipp>
#endif

#endif // BOOST_NETWORK_MESSAGE_WRAPPERS_SOURCE_HPP_20111021
