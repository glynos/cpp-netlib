//          Copyright Dean Michael Berris 2008, 2009.
//                    Glyn Matthews 2009
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_NETWORK_TAG_INCLUDED_20100808
# define BOOST_NETWORK_TAG_INCLUDED_20100808

namespace boost { namespace network { namespace tags {

    struct async {};
    struct http {};
    struct tcp {};
    struct udp {};
    struct sync {};

struct default_string {};
struct default_wstring {};
struct http_default_8bit_tcp_resolve : http,sync,tcp,default_string {};
struct http_default_8bit_udp_resolve : http,sync,udp,default_string {};
struct http_keepalive_8bit_tcp_resolve : http,sync,tcp,default_string {};
struct http_keepalive_8bit_udp_resolve : http,sync,udp,default_string {};
struct http_server : http,default_string {};
struct http_async_8bit_udp_resolve : http,async,udp,default_string {};

typedef default_string default_;

} // namespace tags

} // namespace network

} // namespace boost

#endif // __BOOST_NETWORK_TAGS_INC__
