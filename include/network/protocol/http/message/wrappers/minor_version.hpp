// Copyright 2010 Dean Michael Berris. 
// Copyright 2012 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef NETWORK_PROTOCOL_HTTP_MESSAGE_WRAPPERS_MINOR_VERSION_HPP_20101120
#define NETWORK_PROTOCOL_HTTP_MESSAGE_WRAPPERS_MINOR_VERSION_HPP_20101120

#include <boost/utility/enable_if.hpp>
#include <network/protocol/http/support/is_server.hpp>
#include <boost/cstdint.hpp>

namespace network { namespace http {

    template <class Tag>
    struct basic_request;

    template <class Tag>
    struct minor_version_wrapper {
        basic_request<Tag> const & request;
        explicit minor_version_wrapper(basic_request<Tag> const & request)
        : request(request) {}
        operator boost::uint8_t () {
            return request.http_version_minor;
        }
    };

    template <class Tag>
    inline typename enable_if<is_server<Tag>, minor_version_wrapper<Tag> >::type
    minor_version(basic_request<Tag> const & request) {
        return minor_version_wrapper<Tag>(request);
    }
    
}  // namespace http
}  // namespace network
    
#endif /* NETWORK_PROTOCOL_HTTP_MESSAGE_WRAPPERS_MINOR_VERSION_HPP_20101120 */

