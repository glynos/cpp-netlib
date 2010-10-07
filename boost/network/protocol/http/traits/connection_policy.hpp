#ifndef BOOST_NETWORK_PROTOCOL_HTTP_CONNECTION_POLICY_20091214
#define BOOST_NETWORK_PROTOCOL_HTTP_CONNECTION_POLICY_20091214

//          Copyright Dean Michael Berris 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/tags.hpp>
#include <boost/network/protocol/http/policies/simple_connection.hpp>
#include <boost/network/protocol/http/policies/pooled_connection.hpp>
#include <boost/network/protocol/http/policies/async_connection.hpp>
#include <boost/network/support/is_async.hpp>
#include <boost/network/support/is_simple.hpp>
#include <boost/network/support/is_keepalive.hpp>

namespace boost { namespace network { namespace http {

    template <class Tag>
    struct unsupported_tag;

    template <class Tag, unsigned version_major, unsigned version_minor>
    struct connection_policy :
        mpl::if_<
            is_async<Tag>,
            async_connection_policy<Tag,version_major,version_minor>,
            typename mpl::if_<
                is_simple<Tag>,
                simple_connection_policy<Tag,version_major,version_minor>,
                typename mpl::if_<
                    is_keepalive<Tag>,
                    pooled_connection_policy<Tag,version_major,version_minor>,
                    unsupported_tag<Tag>
                >::type
            >::type
        >
    {};

} // namespace http

} // namespace network

} // namespace boost

#endif // BOOST_NETWORK_PROTOCOL_HTTP_CONNECTION_POLICY_20091214

