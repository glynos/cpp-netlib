#ifndef BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_WRAPPERS_QUERY_IPP_20111204
#define BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_WRAPPERS_QUERY_IPP_20111204

// Copyright 2011 Dean Michael Berris <dberris@google.com>.
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/protocol/http/message/wrappers/query.hpp>
#include <boost/network/uri/uri.hpp>

namespace boost { namespace network { namespace http {

query_wrapper::query_wrapper(request_base const & request)
: request_(request) {}

query_wrapper::operator std::string () const {
  uri::uri uri_;
  request_.get_uri(uri_);
  return query(uri_);
}

}  // namespace http

}  // namespace network

}  // namespace boost

#endif  // BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_WRAPPERS_QUERY_IPP_20111204
