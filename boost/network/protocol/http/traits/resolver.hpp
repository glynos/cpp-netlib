#ifndef BOOST_NETWORK_PROTOCOL_HTTP_TRAITS_RESOLVER_20091214
#define BOOST_NETWORK_PROTOCOL_HTTP_TRAITS_RESOLVER_20091214

//          Copyright Dean Michael Berris 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/tags.hpp>

namespace boost { namespace network { namespace http {

    template <class Tag>
    struct unsupported_tag;

    template <class Tag>
    struct resolver {
        typedef unsupported_tag<Tag> type;
    };

    template <>
    struct resolver<tags::http_default_8bit_tcp_resolve> {
        typedef boost::asio::ip::tcp::resolver type;
    };

    template <>
    struct resolver<tags::http_default_8bit_udp_resolve> {
        typedef boost::asio::ip::udp::resolver type;
    };

} // namespace http

} // namespace network

} // namespace boost

#endif // BOOST_NETWORK_PROTOCOL_HTTP_TRAITS_RESOLVER_20091214

