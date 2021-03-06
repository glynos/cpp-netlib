#ifndef BOOST_NETWORK_SUPPORT_IS_TCP_HPP_20100622
#define BOOST_NETWORK_SUPPORT_IS_TCP_HPP_20100622

// Copyright 2010 (c) Dean Michael Berris
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/mpl/not.hpp>
#include <boost/network/tags.hpp>
#include <boost/utility/enable_if.hpp>

namespace boost {
namespace network {

template <class Tag, class Enable = void>
struct is_tcp : mpl::false_ {};

template <class Tag>
struct is_tcp<Tag,
              typename enable_if<typename Tag::is_tcp>::type> : mpl::true_ {};

}  // namespace network

}  // namespace boost

#endif  // BOOST_NETWORK_SUPPORT_IS_TCP_HPP_20100622
