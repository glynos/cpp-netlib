#ifndef BOOST_NETWORK_PROTOCOL_HTTP_SERVER_HEADER_HPP_20101027
#define BOOST_NETWORK_PROTOCOL_HTTP_SERVER_HEADER_HPP_20101027

// Copyright 2010 Dean Michael Berris. 
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/traits/string.hpp>
#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/assign/list_of.hpp>
#include <algorithm>

namespace boost { namespace network { namespace http {
    
    template <class Tag>
    struct response_header {
        typedef Tag tag;
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

BOOST_FUSION_ADAPT_TPL_STRUCT(
    (Tag),
    (boost::network::http::response_header) (Tag),
    (typename boost::network::string<Tag>::type, name)
    (typename boost::network::string<Tag>::type, value)
    )

#endif /* BOOST_NETWORK_PROTOCOL_HTTP_SERVER_HEADER_HPP_20101027 */
