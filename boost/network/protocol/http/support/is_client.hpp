#ifndef BOOST_NETWORK_PROTOCOL_SUPPORT_IS_CLIENT_HPP_20101118
#define BOOST_NETWORK_PROTOCOL_SUPPORT_IS_CLIENT_HPP_20101118

// Copyright 2010 Dean Michael Berris. 
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/protocol/http/tags.hpp>
#include <boost/type_traits/is_base_of.hpp>

namespace boost { namespace network { namespace http {

    template <class Tag>
    struct is_client : 
        is_base_of<
            tags::client,
            Tag
        >
    {};
    
} /* http */
    
} /* network */
    
} /* boost */

#endif /* BOOST_NETWORK_PROTOCOL_SUPPORT_IS_CLIENT_HPP_20101118 */

