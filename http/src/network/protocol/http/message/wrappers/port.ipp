// Copyright 2011 Dean Michael Berris <dberris@google.com>.
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef NETWORK_PROTOCOL_HTTP_MESSAGE_WRAPPERS_PORT_IPP_20111204
#define NETWORK_PROTOCOL_HTTP_MESSAGE_WRAPPERS_PORT_IPP_20111204

#include <network/protocol/http/message/wrappers/port.hpp>
#include <network/uri.hpp>

namespace network {
namespace http {

port_wrapper::port_wrapper(request_base const & request)
: request_(request) {}

port_wrapper::operator boost::uint16_t () const {
  ::network::uri uri_;
  request_.get_uri(uri_);
  auto optional_port = uri_.port();
  if (!optional_port) {
    auto scheme_ = uri_.scheme();
    assert(scheme_);
    if (*scheme_ == "http") {
      return 80u;
    } else if (*scheme_ == "https") {
      return 443u;
    }
  }
  return std::stoul(std::string(*optional_port));
}

port_wrapper::operator boost::optional<boost::uint16_t> () const {
  ::network::uri uri_;
  request_.get_uri(uri_);
  auto optional_port = uri_.port();
  if (!optional_port) {
    return boost::optional<boost::uint16_t>();
  }
  return std::stoul(std::string(*optional_port));
}

}  // namespace http
}  // namespace network

#endif  // NETWORK_PROTOCOL_HTTP_MESSAGE_WRAPPERS_PORT_IPP_20111204
