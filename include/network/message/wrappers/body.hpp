#ifndef BOOST_NETWORK_MESSAGE_WRAPPERS_BODY_HPP_20110930
#define BOOST_NETWORK_MESSAGE_WRAPPERS_BODY_HPP_20110930

// Copyright 2011 Dean Michael Berris <dberris@google.com>.
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/range/iterator.hpp>
#include <boost/optional.hpp>
#include <network/message/message_base.hpp>

namespace boost { namespace network {

struct body_wrapper {
  explicit body_wrapper(message_base const & message);
  operator std::string () const;
  std::size_t size() const;
  operator iterator_range<std::string::const_iterator> () const;
  std::string::const_iterator begin() const;
  std::string::const_iterator end() const;
 private:
  message_base const & message_;
  mutable optional<std::string> cache_;
};

inline std::ostream & operator<<(std::ostream & os, body_wrapper const & body) {
  os << static_cast<std::string>(body);
  return os;
}

inline body_wrapper const
body(message_base const & message_) {
    return body_wrapper(message_);
}

} // namespace network

} // namespace boost

#ifdef BOOST_NETWORK_NO_LIB
#include <network/message/wrappers/body.ipp>
#endif

#endif // __NETWORK_MESSAGE_WRAPPERS_BODY_HPP__
