//            Copyright (c) Glyn Matthews 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#ifndef __BOOST_NETWORK_TRAITS_OSTRINGSTREAM_INC__
# define __BOOST_NETWORK_TRAITS_OSTRINGSTREAM_INC__


# include <sstream>
# include <boost/network/tags.hpp>


namespace boost {
namespace network {
template <
    class Tag
    >
struct ostringstream {
    typedef void type;
};


template <>
struct ostringstream<tags::default_string> {
    typedef std::ostringstream type;
};


template <>
struct ostringstream<tags::default_wstring> {
    typedef std::wostringstream type;
};
} // namespace network
} // namespace boost


#endif // __BOOST_NETWORK_TRAITS_OSTRINGSTREAM_INC__
