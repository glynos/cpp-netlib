// Copyright 2010 Dean Michael Berris. 
// Copyright 2012 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef NETWORK_PROTOCOL_HTTP_MESSAGE_DIRECTIVES_MINOR_VERSION_HPP_20101120
#define NETWORK_PROTOCOL_HTTP_MESSAGE_DIRECTIVES_MINOR_VERSION_HPP_20101120

#include <network/protocol/http/support/is_server.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/cstdint.hpp>

namespace network {
namespace http {

template <class Tag>
struct basic_request;

struct minor_version_directive {
  boost::uint8_t minor_version;
  explicit minor_version_directive(boost::uint8_t minor_version)
  : minor_version(minor_version) {}
  template <class Tag>
  void operator()(basic_request<Tag> & request) const {
    request.http_version_minor = minor_version;
  }
};

inline minor_version_directive
minor_version(boost::uint8_t minor_version_) {
  return minor_version_directive(minor_version_);
}

}  // namespace http
}  // namespace network
  
#endif /* NETWORK_PROTOCOL_HTTP_MESSAGE_DIRECTIVES_MINOR_VERSION_HPP_20101120 */

