// Copyright 2010 Dean Michael Berris. 
// Copyright 2012 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef NETWORK_PROTOCOL_HTTP_CLIENT_PARAMETERS_HPP_2010127
#define NETWORK_PROTOCOL_HTTP_CLIENT_PARAMETERS_HPP_2010127

#include <network/protocol/http/parameters.hpp>

namespace network { namespace http {

    PARAMETER_NAME(follow_redirects)
    PARAMETER_NAME(cache_resolved)
    PARAMETER_NAME(openssl_certificate)
    PARAMETER_NAME(openssl_verify_path)

    PARAMETER_NAME(request)
    PARAMETER_NAME(body)
    PARAMETER_NAME(content_type)
    PARAMETER_NAME(body_handler)

    PARAMETER_NAME(connection_manager)
    PARAMETER_NAME(connection_factory)

}  // namespace http
}  // namespace network

#endif /* NETWORK_PROTOCOL_HTTP_CLIENT_PARAMETERS_HPP_2010127 */
