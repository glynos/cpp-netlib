
#ifndef BOOST_NETWORK_TRAITS_ISTREAM_HPP_20100924
#define BOOST_NETWORK_TRAITS_ISTREAM_HPP_20100924

// Copyright 2010 (C) Dean Michael Berris
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/support/is_default_string.hpp>
#include <boost/network/support/is_default_wstring.hpp>
#include <boost/network/traits/char.hpp>
#include <boost/mpl/if.hpp>
#include <istream>

namespace boost { namespace network {
    
    template <class Tag>
    struct unsupported_tag;

    template <class Tag>
    struct istream :
        mpl::if_<
            is_default_string<Tag>,
            std::istream,
            typename mpl::if_<
                is_default_wstring<Tag>,
                std::wistream,
                unsupported_tag<Tag>
            >::type
        >
    {};

} /* network */
    
} /* boost */

#endif /* BOOST_NETWORK_TRAITS_ISTREAM_HPP_20100924 */
