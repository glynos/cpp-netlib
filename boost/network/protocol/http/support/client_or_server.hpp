#ifndef BOOST_NETWORK_PROTOCOL_HTTP_SUPPORT_CLIENT_OR_SERVER_HPP_20101127
#define BOOST_NETWORK_PROTOCOL_HTTP_SUPPORT_CLIENT_OR_SERVER_HPP_20101127

// Copyright 2010 Dean Michael Berris. 
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/protocol/http/support/is_server.hpp>
#include <boost/network/protocol/http/support/is_client.hpp>
#include <boost/mpl/if.hpp>

namespace boost { namespace network { namespace http {

    template <class Tag>
    struct client_or_server :
        mpl::if_<
            is_server<Tag>,
            tags::server,
            typename mpl::if_<
                is_client<Tag>,
                tags::client,
                unsupported_tag<Tag>
                >::type
            >
    {};

    
} /* http */
    
} /* network */
    
} /* boost */

#endif /* BOOST_NETWORK_PROTOCOL_HTTP_SUPPORT_CLIENT_OR_SERVER_HPP_20101127 */
