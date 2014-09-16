#ifndef BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_PORT_HPP_20100618
#define BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_PORT_HPP_20100618

// Copyright 2010, 2014 Dean Michael Berris <dberris@google.com>
// Copyright 2010 (c) Sinefunc, Inc.
// Copyright 2014 Google, Inc.
// Copyright 2014 Jussi Lyytinen
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/optional.hpp>
#include <boost/cstdint.hpp>
#include <boost/network/uri/accessors.hpp>

namespace boost {
namespace network {
namespace http {

template <class Tag>
struct basic_request;

namespace impl {

template <class Tag>
struct port_wrapper {
  basic_request<Tag> const& message_;

  port_wrapper(basic_request<Tag> const& message) : message_(message) {}

  typedef typename basic_request<Tag>::port_type port_type;

  operator port_type() const { return message_.port(); }

#if !defined(_MSC_VER)
  // Because of a breaking change in Boost 1.56 to boost::optional, implicit
  // conversions no longer work correctly with MSVC. The conversion therefore
  // has to be done explicitly with as_optional(). This method is here just
  // to maintain backwards compatibility with compilers not affected by the
  // change.
  operator boost::optional<boost::uint16_t>() const {
    return uri::port_us(message_.uri());
  }
#endif
  
  boost::optional<boost::uint16_t> as_optional() const {
    return uri::port_us(message_.uri());
  }

};

}  // namespace impl

template <class Tag>
inline impl::port_wrapper<Tag> port(basic_request<Tag> const& request) {
  return impl::port_wrapper<Tag>(request);
}

}  // namespace http
}  // namespace network
}  // namespace boost

#endif  // BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_PORT_HPP_20100618
