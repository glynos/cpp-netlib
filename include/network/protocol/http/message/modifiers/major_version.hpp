// Copyright 2010 Dean Michael Berris. 
// Copyright 2012 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef NETWORK_PROTOCOL_HTTP_MESSAGE_MODIFIERS_MAJOR_VERSION_HPP_20101120
#define NETWORK_PROTOCOL_HTTP_MESSAGE_MODIFIERS_MAJOR_VERSION_HPP_20101120

#include <network/protocol/http/support/is_server.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/cstdint.hpp>

namespace network {
namespace http {

template <class Tag>
struct basic_request;

template <class Tag>
inline typename enable_if<is_server<Tag>, void>::type
major_version(basic_request<Tag> & request, boost::uint8_t major_version_) {
    request.http_version_major = major_version_;
}
    
}  // namespace http
}  // namespace network
    
#endif /* NETWORK_PROTOCOL_HTTP_MESSAGE_MODIFIERS_MAJOR_VERSION_HPP_20101120 */
