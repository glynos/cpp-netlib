#ifndef BOOST_NETWORK_SUPPORT_IS_SYNC_HPP_20100623
#define BOOST_NETWORK_SUPPORT_IS_SYNC_HPP_20100623

// Copyright 2010 (c) Dean Michael Berris
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/tags.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/static_assert.hpp>

namespace boost { namespace network {

    template <class Tag>
    struct is_sync :
        is_base_of<
            tags::sync,
            Tag
        >
    {};

} // namespace network

} // namespace boost

#endif // BOOST_NETWORK_SUPPORT_IS_SYNC_HPP_20100623