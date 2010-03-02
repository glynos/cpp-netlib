//            Copyright (c) Dean Michael Berris 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_NETWORK_PROTOCOL_HTTP_TRAITS_CONECTION_KEEPALIVE_20091218
#define BOOST_NETWORK_PROTOCOL_HTTP_TRAITS_CONECTION_KEEPALIVE_20091218

#include <boost/network/tags.hpp>

namespace boost { namespace network { namespace http {

    template <class Tag>
    struct connection_keepalive {
        static bool const value = false;
    };

    template <>
    struct connection_keepalive<tags::http_keepalive_8bit_udp_resolve> {
        static bool const value = true;
    };

    template <>
    struct connection_keepalive<tags::http_keepalive_8bit_tcp_resolve> {
        static bool const value = true;
    };
    
} /* http */
    
} /* network */
    
} /* boost */

#endif // BOOST_NETWORK_PROTOCOL_HTTP_TRAITS_CONECTION_KEEPALIVE_20091218

