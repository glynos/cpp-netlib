#ifndef BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_WRAPPERS_STATUS_MESSAGE_HPP_20100603
#define BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_WRAPPERS_STATUS_MESSAGE_HPP_20100603

// Copyright 2010 (c) Dean Michael Berris
// Copyright 2010 (c) Sinefunc, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/protocol/http/response_base.hpp>

namespace boost { namespace network { namespace http {

namespace impl {

struct status_message_wrapper {
  explicit status_message_wrapper(response_base & response_);
  operator std::string () const;
 private:
  response_base & response_;
  mutable optional<std::string> cache_;
};

inline std::ostream & operator<<(std::ostream & os, status_message_wrapper const & status_message) {
  return os << static_cast<std::string>(status_message);
}

} // namespace impl

inline 
impl::status_message_wrapper
status_message(response_base & response) {
    return impl::status_message_wrapper(response);
}

} // namespace http

} // namespace network

} // namespace boost

#endif // BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_WRAPPER_STATUS_MESSAGE_HPP_20100603
