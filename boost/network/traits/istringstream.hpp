//            Copyright (c) Glyn Matthews 2009.
//         Copyright (c) Dean Michael Berris 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_NETWORK_TRAITS_ISTRINGSTREAM_INC
# define BOOST_NETWORK_TRAITS_ISTRINGSTREAM_INC

# include <sstream>
# include <boost/network/tags.hpp>
#include <boost/network/support/is_default_string.hpp>
#include <boost/network/support/is_default_wstring.hpp>
#include <boost/mpl/if.hpp>

namespace boost { namespace network {

    template <class Tag>
    struct unsupported_tag;

    template <class Tag>
    struct istringstream :
        mpl::if_<
            is_default_string<Tag>,
            std::istringstream,
            typename mpl::if_<
                is_default_wstring<Tag>,
                std::basic_istringstream<wchar_t>,
                unsupported_tag<Tag>
            >::type
        >
    {};

} // namespace network

} // namespace boost


#endif // BOOST_NETWORK_TRAITS_ISTRINGSTREAM_INC

