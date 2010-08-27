//            Copyright (c) Dean Michael Berris 2008, 2009.
//                          Glyn Matthews 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_NETWORK_TRAITS_STRING_INC
#define BOOST_NETWORK_TRAITS_STRING_INC

# include <string>
# include <boost/network/tags.hpp>
# include <boost/network/support/is_default_string.hpp>
# include <boost/network/support/is_default_wstring.hpp>

#ifndef BOOST_NETWORK_DEFAULT_STRING
#define BOOST_NETWORK_DEFAULT_STRING std::string
#endif

#ifndef BOOST_NETWORK_DEFAULT_WSTRING
#define BOOST_NETWORK_DEFAULT_WSTRING std::wstring
#endif

namespace boost { namespace network {

    template <class Tag>
    struct unsupported_tag;

    template <class Tag>
    struct string
        : mpl::if_<
            is_default_string<Tag>,
            BOOST_NETWORK_DEFAULT_STRING,
            typename mpl::if_<
                is_default_wstring<Tag>,
                BOOST_NETWORK_DEFAULT_WSTRING,
                unsupported_tag<Tag>
            >::type
        >
    {};

} // namespace network

} // namespace boost


#endif // BOOST_NETWORK_TRAITS_STRING_INC
