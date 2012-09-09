#ifndef BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_WRAPPERS_PORT_IPP_20111204
#define BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_WRAPPERS_PORT_IPP_20111204

// Copyright 2011 Dean Michael Berris <dberris@google.com>.
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <network/protocol/http/message/wrappers/port.hpp>
#include <network/uri.hpp>

namespace boost { namespace network { namespace http {

port_wrapper::port_wrapper(request_base const & request)
: request_(request) {}

port_wrapper::operator boost::uint16_t () const {
  ::network::uri uri_;
  request_.get_uri(uri_);
  optional<boost::uint16_t> optional_port = port_us(uri_);
  if (!optional_port) {
    std::string scheme_ = scheme(uri_);
    if (scheme_ == "http") {
      return 80u;
    } else if (scheme_ == "https") {
      return 443u;
    }
  }
  return optional_port ? *optional_port : 80u;
}

port_wrapper::operator optional<boost::uint16_t> () const {
  ::network::uri uri_;
  request_.get_uri(uri_);
  return port_us(uri_);
}

}  // namespace http

}  // namespace network

}  // namespace boost

#endif  // BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_WRAPPERS_PORT_IPP_20111204
