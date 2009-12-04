//            Copyright (c) Glyn Matthews 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#ifndef __BOOST_NETWORK_TRAITS_HEADERS_CONTAINER_INC__
# define __BOOST_NETWORK_TRAITS_HEADERS_CONTAINER_INC__


# include <map>
# include <boost/network/tags.hpp>
# include <boost/network/traits/string.hpp>


namespace boost {
namespace network {
template <
    class Tag
    >
struct headers_container {
    typedef std::multimap<
        typename string<Tag>::type,
        typename string<Tag>::type
        > type;
};


// template <>
// struct headers_container<tags::default_string> {
//     typedef std::multimap<
//         string<tags::default_string>::type,
//         string<tags::default_string>::type
//         > type;
// };
// 
// 
// template <>
// struct headers_container<tags::default_wstring> {
//     typedef std::multimap<
//         string<tags::default_wstring>::type,
//         string<tags::default_wstring>::type
//         > type;
// };
} // namespace network
} // namespace boost


#endif // __BOOST_NETWORK_TRAITS_HEADERS_CONTAINER_INC__
