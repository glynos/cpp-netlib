#ifndef BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_WRAPPERS_STATUS_IPP_20120311
#define BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_WRAPPERS_STATUS_IPP_20120311

// Copyright 2012 Dean Michael Berris <dberris@google.com>.
// Copyright 2012 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <network/protocol/http/message/wrappers/status.hpp>

namespace boost { namespace network { namespace http {

status_wrapper::status_wrapper(response_base &response)
: response_(response)
{}

status_wrapper::operator uint16_t () const {
  uint16_t status;
  response_.get_status(status);
  return status;
}

}  // namespace http

}  // namespace network

}  // namespace boost

#endif  // BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_WRAPPERS_STATUS_IPP_20120311
