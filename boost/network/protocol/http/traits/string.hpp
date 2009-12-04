//            Copyright (c) Glyn Matthews 2009.
//                          Dean Michael Berris 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#ifndef __BOOST_NETWORK_PROTOCOL_HTTP_TRAITS_STRING_INC__
# define __BOOST_NETWORK_PROTOCOL_HTTP_TRAITS_STRING_INC__


# include <string>
# include <boost/network/traits/string.hpp>

namespace boost { namespace network { 

    namespace tags {
        struct http;
    } // namespace tags
    
    template <>
    struct string<tags::http> {
        typedef std::string type;
    };

} // namespace network

} // namespace boost

#endif // __BOOST_NETWORK_PROTOCOL_HTTP_TRAITS_STRING_INC__
