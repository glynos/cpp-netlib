//            Copyright (c) Glyn Matthews 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_NETWORK_TRAITS_OSTRINGSTREAM_INC
# define BOOST_NETWORK_TRAITS_OSTRINGSTREAM_INC

# include <sstream>
# include <boost/network/tags.hpp>

namespace boost { namespace network {

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

    template <>
    struct ostringstream<tags::http_default_8bit_tcp_resolve> {
        typedef std::ostringstream type;
    };

} // namespace network

} // namespace boost


#endif // BOOST_NETWORK_TRAITS_OSTRINGSTREAM_INC
