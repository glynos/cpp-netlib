#ifndef BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_WRAPPERS_VERSION_HPP_20100603
#define BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_WRAPPERS_VERSION_HPP_20100603

// Copyright 2010-2012 (c) Dean Michael Berris
// Copyright 2010 (c) Sinefunc, Inc.
// Copyright 2012 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/protocol/http/response/response_base.hpp>

namespace boost { namespace network { namespace http {

struct version_wrapper {
  explicit version_wrapper(response_base & response_);
  operator std::string() const;
 private:
  response_base & response_;
  mutable optional<std::string> cache_;
};

inline std::ostream & operator<< (std::ostream & os, version_wrapper const & version) {
  return os << static_cast<std::string>(version);
}

inline 
version_wrapper
version(response_base & response) {
  return version_wrapper(response);
}

} // namespace http

} // namespace network

} // namespace boost

#endif // BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_WRAPPERS_VERSION_HPP_20100603
