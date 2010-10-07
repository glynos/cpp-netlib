//          Copyright Dean Michael Berris 2008, 2009.
//                    Glyn Matthews 2009
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_NETWORK_TAG_INCLUDED_20100808
#define BOOST_NETWORK_TAG_INCLUDED_20100808

#include <boost/mpl/vector.hpp>
#include <boost/mpl/inherit.hpp>
#include <boost/mpl/inherit_linearly.hpp>
#include <boost/mpl/placeholders.hpp>

namespace boost { namespace network { namespace tags {

    struct async { int unused; };
    struct http { int unused; };
    struct tcp { int unused; };
    struct udp { int unused; };
    struct sync { int unused; };
    struct keepalive { int unused; };
    struct simple { int unused; };
    struct default_string { int unused; };
    struct default_wstring { int unused; };

    // FIXME move out these HTTP-only tags into different files

    typedef mpl::vector<http, simple, sync, tcp, default_string> http_default_8bit_tcp_resolve_tags;
    typedef mpl::vector<http, simple, sync, udp, default_string> http_default_8bit_udp_resolve_tags;
    typedef mpl::vector<http, keepalive, sync, tcp, default_string> http_keepalive_8bit_tcp_resolve_tags;
    typedef mpl::vector<http, keepalive, sync, udp, default_string> http_keepalive_8bit_udp_resolve_tags;
    typedef mpl::vector<http, simple, async, udp, default_string> http_async_8bit_udp_resolve_tags;
    typedef mpl::vector<http, simple, async, tcp, default_string> http_async_8bit_tcp_resolve_tags;
    typedef mpl::vector<http, simple, sync, default_string> http_server_tags;

    template <class Tag>
    struct components;

    // FIXME make this a preprocessor macro!
    typedef mpl::inherit_linearly<
        http_default_8bit_tcp_resolve_tags,
        mpl::inherit<mpl::placeholders::_1, mpl::placeholders::_2>
        >::type http_default_8bit_tcp_resolve;

    template <>
    struct components<http_default_8bit_tcp_resolve> {
        typedef http_default_8bit_tcp_resolve_tags type;
    };

    typedef mpl::inherit_linearly<
        http_default_8bit_udp_resolve_tags,
        mpl::inherit<mpl::placeholders::_1, mpl::placeholders::_2>
        >::type http_default_8bit_udp_resolve;

    template <>
    struct components<http_default_8bit_udp_resolve> {
        typedef http_default_8bit_udp_resolve_tags type;
    };

    typedef mpl::inherit_linearly<
        http_keepalive_8bit_tcp_resolve_tags,
        mpl::inherit<mpl::placeholders::_1, mpl::placeholders::_2>
        >::type http_keepalive_8bit_tcp_resolve;

    template <>
    struct components<http_keepalive_8bit_tcp_resolve> {
        typedef http_keepalive_8bit_tcp_resolve_tags type;
    };

    typedef mpl::inherit_linearly<
        http_keepalive_8bit_udp_resolve_tags,
        mpl::inherit<mpl::placeholders::_1, mpl::placeholders::_2>
        >::type http_keepalive_8bit_udp_resolve;

    template <>
    struct components<http_keepalive_8bit_udp_resolve> {
        typedef http_keepalive_8bit_udp_resolve_tags type;
    };

    typedef mpl::inherit_linearly<
        http_server_tags,
        mpl::inherit<mpl::placeholders::_1, mpl::placeholders::_2>
        >::type http_server;

    template <>
    struct components<http_server> {
        typedef http_server_tags type;
    };

    typedef mpl::inherit_linearly<
        http_async_8bit_udp_resolve_tags,
        mpl::inherit<mpl::placeholders::_1, mpl::placeholders::_2>
        >::type http_async_8bit_udp_resolve;

    template <>
    struct components<http_async_8bit_udp_resolve> {
        typedef http_async_8bit_udp_resolve_tags type;
    };

    typedef mpl::inherit_linearly<
        http_async_8bit_tcp_resolve_tags,
        mpl::inherit<mpl::placeholders::_1, mpl::placeholders::_2>
        >::type http_async_8bit_tcp_resolve;

    template <>
    struct components<http_async_8bit_tcp_resolve> {
        typedef http_async_8bit_tcp_resolve_tags type;
    };

    typedef default_string default_;

} // namespace tags

} // namespace network

} // namespace boost

#endif // __BOOST_NETWORK_TAGS_INC__
