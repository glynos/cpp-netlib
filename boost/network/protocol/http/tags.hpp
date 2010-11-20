#ifndef BOOST_NETWORK_PROTOCOL_HTTP_TAGS_HPP_20101019
#define BOOST_NETWORK_PROTOCOL_HTTP_TAGS_HPP_20101019

// Copyright Dean Michael Berris 2010.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/tags.hpp>

namespace boost { namespace network { namespace http { namespace tags {

    struct http {};
    struct keepalive {};
    struct simple {};
    struct server {};

    using namespace boost::network::tags;

    template <class Tag>
    struct components;
    
    typedef mpl::vector<http, simple, sync, tcp, default_string> http_default_8bit_tcp_resolve_tags;
    typedef mpl::vector<http, simple, sync, udp, default_string> http_default_8bit_udp_resolve_tags;
    typedef mpl::vector<http, keepalive, sync, tcp, default_string> http_keepalive_8bit_tcp_resolve_tags;
    typedef mpl::vector<http, keepalive, sync, udp, default_string> http_keepalive_8bit_udp_resolve_tags;
    typedef mpl::vector<http, simple, async, udp, default_string> http_async_8bit_udp_resolve_tags;
    typedef mpl::vector<http, simple, async, tcp, default_string> http_async_8bit_tcp_resolve_tags;
    typedef mpl::vector<http, simple, sync, pod, default_string, server> http_server_tags;
    typedef mpl::vector<http, simple, sync, pod, default_string, server> http_async_server_tags;

    BOOST_NETWORK_DEFINE_TAG(http_default_8bit_tcp_resolve);
    BOOST_NETWORK_DEFINE_TAG(http_default_8bit_udp_resolve);
    BOOST_NETWORK_DEFINE_TAG(http_keepalive_8bit_tcp_resolve);
    BOOST_NETWORK_DEFINE_TAG(http_keepalive_8bit_udp_resolve);
    BOOST_NETWORK_DEFINE_TAG(http_async_8bit_udp_resolve);
    BOOST_NETWORK_DEFINE_TAG(http_async_8bit_tcp_resolve);
    BOOST_NETWORK_DEFINE_TAG(http_server);
    BOOST_NETWORK_DEFINE_TAG(http_async_server);

} /* tags */
    
} /* http */
    
} /* network */
    
} /* boost */

#endif /* BOOST_NETWORK_PROTOCOL_HTTP_TAGS_HPP_20101019 */
