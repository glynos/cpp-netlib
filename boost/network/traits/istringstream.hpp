//            Copyright (c) Glyn Matthews 2009.
//         Copyright (c) Dean Michael Berris 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_NETWORK_TRAITS_ISTRINGSTREAM_INC
# define BOOST_NETWORK_TRAITS_ISTRINGSTREAM_INC

# include <sstream>
# include <boost/network/tags.hpp>

namespace boost { namespace network {

    template <class Tag>
    struct unsupported_tag;

    template <class Tag>
    struct istringstream {
        typedef unsupported_tag<Tag> type;
    };

    template <>
    struct istringstream<tags::default_string> {
        typedef std::istringstream type;
    };


    template <>
    struct istringstream<tags::default_wstring> {
        typedef std::wistringstream type;
    };

    template <>
    struct istringstream<tags::http_default_8bit_tcp_resolve> {
        typedef std::istringstream type;
    };

    template <>
    struct istringstream<tags::http_default_8bit_udp_resolve> {
        typedef std::istringstream type;
    };

    template <>
    struct istringstream<tags::http_keepalive_8bit_tcp_resolve> {
        typedef std::istringstream type;
    };

    template <>
    struct istringstream<tags::http_keepalive_8bit_udp_resolve> {
        typedef std::istringstream type;
    };

} // namespace network

} // namespace boost


#endif // BOOST_NETWORK_TRAITS_ISTRINGSTREAM_INC

