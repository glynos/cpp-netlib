#ifndef BOOST_NETWORK_SUPPORT_IS_HTTP_HPP_20100622
#define BOOST_NETWORK_SUPPORT_IS_HTTP_HPP_20100622

// Copyright 2010 (c) Dean Michael Berris
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/tags.hpp>
#include <boost/type_traits/is_base_of.hpp>

namespace boost { namespace network {

    template <class Tag>
    struct is_http :
        is_base_of<
            tags::http,
            Tag
        >
    {};

} // namespace network

} // namespace boost

#endif // BOOST_NETWORK_SUPPORT_IS_HTTP_HPP_20100622
