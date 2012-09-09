#ifndef BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_WRAPPERS_IPP_20111204
#define BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_WRAPPERS_IPP_20111204

// Copyright 2011 Dean Michael Berris <dberris@google.com>.
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <network/protocol/http/message/wrappers/host.hpp>
#include <network/uri.hpp>

namespace boost { namespace network { namespace http {

host_wrapper::host_wrapper(request_base const & request)
: request_(request) {}

host_wrapper::operator std::string () const {
  ::network::uri uri_;
  request_.get_uri(uri_);
  return host(uri_);
}

}  // namespace http

}  // namespace network

}  // namespace boost

#endif  // BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_WRAPPERS_IPP_20111204
