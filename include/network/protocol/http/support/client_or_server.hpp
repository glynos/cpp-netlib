// Copyright 2010 Dean Michael Berris. 
// Copyright 2012 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef NETWORK_PROTOCOL_HTTP_SUPPORT_CLIENT_OR_SERVER_HPP_20101127
#define NETWORK_PROTOCOL_HTTP_SUPPORT_CLIENT_OR_SERVER_HPP_20101127

#include <network/protocol/http/support/is_server.hpp>
#include <network/protocol/http/support/is_client.hpp>
#include <boost/mpl/if.hpp>

namespace network { namespace http {
    
    template <class Tag>
    struct unsupported_tag;
    
    template <class Tag, class Enable = void>
    struct client_or_server
    {
        typedef unsupported_tag<Tag> type;
    };
    
    template <class Tag>
    struct client_or_server<Tag, typename enable_if<is_server<Tag> >::type>
    {
        typedef tags::server type;
    };
    
    template <class Tag>
    struct client_or_server<Tag, typename enable_if<is_client<Tag> >::type>
    {
        typedef tags::client type;
    };
    
}  // namespace http
}  // namespace network
    
#endif /* NETWORK_PROTOCOL_HTTP_SUPPORT_CLIENT_OR_SERVER_HPP_20101127 */
