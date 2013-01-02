// Copyright 2010 Dean Michael Berris. 
// Copyright 2012 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef NETWORK_PROTOCOL_HTTP_MESSAGE_DIRECTIVES_MAJOR_VERSION_HPP_20101120
#define NETWORK_PROTOCOL_HTTP_MESSAGE_DIRECTIVES_MAJOR_VERSION_HPP_20101120

#include <boost/cstdint.hpp>

namespace network { namespace http {

struct major_version_directive {
  boost::uint8_t major_version;
  explicit major_version_directive(boost::uint8_t major_version);
  void operator()(request_base &request) const;
};

inline major_version_directive
major_version(boost::uint8_t major_version_) {
  return major_version_directive(major_version_);
}
    
}  // namespace http
}  // namespace network

#endif /* NETWORK_PROTOCOL_HTTP_MESSAGE_DIRECTIVES_MAJOR_VERSION_HPP_20101120 */
