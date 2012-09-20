// Copyright 2010 Dean Michael Berris. 
// Copyright 2012 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef NETWORK_PROTOCOL_HTTP_MESSAGE_MODIFIERS_METHOD_HPP_20101118
#define NETWORK_PROTOCOL_HTTP_MESSAGE_MODIFIERS_METHOD_HPP_20101118

#include <network/protocol/http/request/request_base.hpp>

namespace network { namespace http {
    
inline void method(request_base & request, std::string const & method_) {
  request.set_method(method_);
}

}  // namespace http
}  // namespace network
    
#endif /* NETWORK_PROTOCOL_HTTP_MESSAGE_MODIFIERS_METHOD_HPP_20101118 */
