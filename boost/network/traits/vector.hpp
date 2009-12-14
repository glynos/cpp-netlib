//            Copyright (c) Dean Michael Berris 2008, 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_NETWORK_TRAITS_VECTOR_HPP
#define BOOST_NETWORK_TRAITS_VECTOR_HPP

#include <vector>

namespace boost { namespace network {

    template <class Tag>
    struct unsupported_tag;

    template <class Tag>
    struct vector {

        template <class Type>
        struct apply {
            typedef unsupported_tag<Tag> type;
        };

    };

    template <>
    struct vector<tags::pod> {

        template <class Type>
        struct apply {
            typedef std::vector<Type> type;
        };

    };

} // namespace network

} // namespace boost

#endif // BOOST_NETWORK_TRAITS_VECTOR_HPP

