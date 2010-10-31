#ifndef BOOST_NETWORK_PROTOCOL_HTTP_SERVER_HEADER_HPP_20101027
#define BOOST_NETWORK_PROTOCOL_HTTP_SERVER_HEADER_HPP_20101027

// Copyright 2010 Dean Michael Berris. 
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/traits/string.hpp>
#include <algorithm>

namespace boost { namespace network { namespace http {
    
    template <class Tag>
    struct response_header {
        typedef typename string<Tag>::type string_type;
        string_type name, value;
    };


    template <class Tag>
    void swap(response_header<Tag> & l, response_header<Tag> & r) {
        std::swap(l.name, r.name);
        std::swap(l.value, r.value);
    }

} /* http */

} /* network */
    
} /* boost */

#endif /* BOOST_NETWORK_PROTOCOL_HTTP_SERVER_HEADER_HPP_20101027 */
