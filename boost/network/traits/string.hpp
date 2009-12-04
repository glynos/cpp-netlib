//            Copyright (c) Dean Michael Berris 2008, 2009.
//                          Glyn Matthews 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#ifndef __BOOST_NETWORK_TRAITS_STRING_INC__
# define __BOOST_NETWORK_TRAITS_STRING_INC__


# include <string>
# include <boost/network/tags.hpp>

namespace boost { namespace network {

    template <class Tag>
    struct string { 
        typedef void type;
    };

    template <>
    struct string<tags::default_string> {
        typedef std::string type;
    };


    template <>
    struct string<tags::default_wstring> {
        typedef std::wstring type;
    };

} // namespace network

} // namespace boost


#endif // __BOOST_NETWORK_TRAITS_STRING_INC__
