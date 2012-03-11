#ifndef BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_WRAPPERS_STATUS_HPP_20100603
#define BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_WRAPPERS_STATUS_HPP_20100603

// Copyright 2010 (c) Dean Michael Berris
// Copyright 2010 (c) Sinefunc, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/cstdint.hpp>

namespace boost { namespace network { namespace http {

namespace impl {

struct status_wrapper {
  explicit status_wrapper(response_base & response_);
  operator std::string () const;
  operator uint16_t () const;
 private:
  response_base & response_;
};

} // namespace impl

inline
impl::status_wrapper
status(response_base & response) {
    return impl::status_wrapper(response);
}

} // namespace http

} // namespace network

} // namespace boost

#endif // BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_WRAPPERS_STATUS_HPP_20100603
