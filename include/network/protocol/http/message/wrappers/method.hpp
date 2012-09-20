// Copyright 2010 Dean Michael Berris. 
// Copyright 2012 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef NETWORK_PROTOCOL_HTTP_MESSAGE_WRAPPERS_METHOD_HPP_20101118
#define NETWORK_PROTOCOL_HTTP_MESSAGE_WRAPPERS_METHOD_HPP_20101118

#include <network/protocol/http/request/request_base.hpp>

namespace network {
namespace http {

struct method_wrapper {
  explicit method_wrapper(request_base & message);
  operator std::string () const;
 private:
  request_base & message_;
};

inline method_wrapper const
method(request_base & message) {
  return method_wrapper(message);
}
    
}  // namespace http
}  // namespace network

#endif /* NETWORK_PROTOCOL_HTTP_MESSAGE_WRAPPERS_METHOD_HPP_20101118 */
