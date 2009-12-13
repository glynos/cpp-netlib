#ifndef BOOST_NETWORK_PROTOCOL_HTTP_RESOLVER_POLICY_20091214
#define BOOST_NETWORK_PROTOCOL_HTTP_RESOLVER_POLICY_20091214

//          Copyright Dean Michael Berris 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/tags.hpp>
#include <boost/network/protocol/http/policies/sync_resolver.hpp>

namespace boost { namespace network { namespace http {

    template <class Tag>
    struct unsupported_tag;

    template <class Tag>
    struct resolver_policy {
        typedef unsupported_tag<Tag> type;
    };

    template <>
    struct resolver_policy<tags::http> {
        typedef policies::sync_resolver<tags::http> type;
    };

} // namespace http

} // namespace network

} // namespace boost

#endif // BOOST_NETWORK_PROTOCOL_HTTP_RESOLVER_POLICY_20091214

