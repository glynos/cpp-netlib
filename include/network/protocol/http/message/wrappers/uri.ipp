#ifndef BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_WRAPPERS_URI_IPP_20120315
#define BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_WRAPPERS_URI_IPP_20120315

// Copyright 2012 Dean Michael Berris <dberris@google.com>.
// Copyright 2012 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <network/protocol/http/message/wrappers/uri.hpp>

namespace boost { namespace network { namespace http {

uri_wrapper::uri_wrapper(request_base const & request_)
: request_(request_) {
  // do nothing here
}

uri_wrapper::operator std::string() const {
  std::string uri;
  request_.get_uri(uri);
  return uri;
}

uri_wrapper::operator ::network::uri() const {
  ::network::uri uri;
  request_.get_uri(uri);
  return uri;
}

}  // namespace http

}  // namespace network

}  // namespace boost


#endif // BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_WRAPPERS_URI_IPP_20120315
