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

    struct async {};
    struct http {};
    struct tcp {};
    struct udp {};
    struct sync {};
    struct keepalive {};
    struct simple {};
    struct default_string {};
    struct default_wstring {};

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

    // Tag Definition Macro Helper
#define BOOST_NETWORK_DEFINE_TAG(name)                                      \
    typedef mpl::inherit_linearly<                                          \
                name##_tags,                                                \
                mpl::inherit<mpl::placeholders::_1, mpl::placeholders::_2>  \
            >::type name;                                                   \
    template <> struct components<name> { typedef name##_tags type; };     \

    BOOST_NETWORK_DEFINE_TAG(http_default_8bit_tcp_resolve);
    BOOST_NETWORK_DEFINE_TAG(http_default_8bit_udp_resolve);
    BOOST_NETWORK_DEFINE_TAG(http_keepalive_8bit_tcp_resolve);
    BOOST_NETWORK_DEFINE_TAG(http_keepalive_8bit_udp_resolve);
    BOOST_NETWORK_DEFINE_TAG(http_async_8bit_udp_resolve);
    BOOST_NETWORK_DEFINE_TAG(http_async_8bit_tcp_resolve);
    BOOST_NETWORK_DEFINE_TAG(http_server);

    typedef default_string default_;

#undef BOOST_NETWORK_DEFINE_TAG

} // namespace tags

} // namespace network

} // namespace boost

#endif // __BOOST_NETWORK_TAGS_INC__
