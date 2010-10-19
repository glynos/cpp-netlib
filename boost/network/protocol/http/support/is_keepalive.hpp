#ifndef BOOST_NETWORK_SUPPORT_IS_KEEPALIVE_HPP_20100927
#define BOOST_NETWORK_SUPPORT_IS_KEEPALIVE_HPP_20100927

// Copyright 2010 (c) Dean Michael Berris
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/protocol/http/tags.hpp>
#include <boost/type_traits/is_base_of.hpp>

namespace boost { namespace network { namespace http {
    
    template <class Tag>
    struct unsupported_tag;

    template <class Tag>
    struct is_keepalive :
        is_base_of<
            http::tags::keepalive
            , Tag
        >
    {};

} /* http */

} /* network */
    
} /* boost */

#endif /* BOOST_NETWORK_SUPPORT_IS_KEEPALIVE_HPP_20100927 */
