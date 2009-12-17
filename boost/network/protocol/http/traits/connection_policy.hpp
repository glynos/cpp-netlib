#ifndef BOOST_NETWORK_PROTOCOL_HTTP_CONNECTION_POLICY_20091214
#define BOOST_NETWORK_PROTOCOL_HTTP_CONNECTION_POLICY_20091214

//          Copyright Dean Michael Berris 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/tags.hpp>
#include <boost/network/protocol/http/policies/simple_connection.hpp>
#include <boost/network/protocol/http/policies/pooled_connection.hpp>

namespace boost { namespace network { namespace http {

    template <class Tag>
    struct unsupported_tag;

    template <class Tag, unsigned version_major, unsigned version_minor>
    struct connection_policy {
        typedef unsupported_tag<Tag> type;
    };

    template <unsigned version_major, unsigned version_minor>
    struct connection_policy<tags::http_default_8bit_udp_resolve, version_major, version_minor> {
        typedef simple_connection_policy<tags::http_default_8bit_udp_resolve, version_major, version_minor> type;
    };

    template <unsigned version_major, unsigned version_minor>
    struct connection_policy<tags::http_default_8bit_tcp_resolve, version_major, version_minor> {
        typedef simple_connection_policy<tags::http_default_8bit_tcp_resolve, version_major, version_minor> type;
    };

    template <unsigned version_major, unsigned version_minor>
    struct connection_policy<tags::http_keepalive_8bit_udp_resolve, version_major, version_minor> {
        typedef pooled_connection_policy<tags::http_keepalive_8bit_udp_resolve, version_major, version_minor> type;
    };

    template <unsigned version_major, unsigned version_minor>
    struct connection_policy<tags::http_keepalive_8bit_tcp_resolve, version_major, version_minor> {
        typedef pooled_connection_policy<tags::http_keepalive_8bit_tcp_resolve, version_major, version_minor> type;
    };

} // namespace http

} // namespace network

} // namespace boost

#endif // BOOST_NETWORK_PROTOCOL_HTTP_CONNECTION_POLICY_20091214

