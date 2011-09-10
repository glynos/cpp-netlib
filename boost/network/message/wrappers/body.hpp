
//          Copyright Dean Michael Berris 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef __NETWORK_MESSAGE_WRAPPERS_BODY_HPP__
#define __NETWORK_MESSAGE_WRAPPERS_BODY_HPP__

#include <boost/range/iterator.hpp>
#include <boost/optional.hpp>
#include <boost/network/message_base.hpp>

namespace boost { namespace network {

namespace impl {

struct body_wrapper {
  explicit body_wrapper(message_base & message_);
  operator std::string () const;
  std::size_t size() const;
  operator iterator_range<std::string::const_iterator> () const;
  std::string::const_iterator begin() const;
  std::string::const_iterator end() const;
 private:
  message_base & message_;
  mutable optional<std::string> cache_;
};

inline std::ostream & operator<<(std::ostream & os, body_wrapper const & body) {
  os << static_cast<std::string>(body);
  return os;
}

} // namespace impl

inline impl::body_wrapper const
body(message_base & message_) {
    return impl::body_wrapper(message_);
}

} // namespace network

} // namespace boost

#ifdef BOOST_NETWORK_NO_LIB
#include <boost/network/message/wrappers/body.ipp>
#endif

#endif // __NETWORK_MESSAGE_WRAPPERS_BODY_HPP__

