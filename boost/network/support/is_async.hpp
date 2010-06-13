#ifndef BOOST_NETWORK_SUPPORT_IS_ASYNC_HPP_20100608
#define BOOST_NETWORK_SUPPORT_IS_ASYNC_HPP_20100608

// Copyright 2010 (c) Dean Michael Berris
// Copyright 2010 (c) Sinefunc, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/tags.hpp>
#include <boost/type_traits/is_base_of.hpp>

namespace boost { namespace network {

    template <class Tag>
    struct is_async :
        is_base_of<
            tags::async,
            Tag
        >
    {};

} // namespace network

} // namespace boost

#endif //BOOST_NETWORK_SUPPORT_IS_ASYNC_HPP_2010608
