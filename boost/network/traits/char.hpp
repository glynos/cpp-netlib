//            Copyright (c) Dean Michael Berris 2008, 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_NETWORK_TRAITS_CHAR_HPP
#define BOOST_NETWORK_TRAITS_CHAR_HPP

#include <boost/network/tags.hpp>

namespace boost { namespace network {

    template <class Tag>
    struct unsupported_tag;

    template <class Tag>
    struct char_ {
        typedef unsupported_tag<Tag> type;
    };

    template <>
    struct char_<tags::http_default_8bit_tcp_resolve> {
        typedef char type;
    };

    template <>
    struct char_<tags::http_default_8bit_udp_resolve> {
        typedef char type;
    };

} // namespace network

} // namespace boost

#endif // BOOST_NETWORK_TRAITS_CHAR_HPP

