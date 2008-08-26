
//          Copyright Dean Michael Berris 2008.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef NETWORK_MESSAGE_TRAITS_HEADER_CONTAINER_HPP
#define NETWORK_MESSAGE_TRAITS_HEADER_CONTAINER_HPP

#include <map>
#include <boost/network/message/traits/string.hpp>
#include <boost/network/message/tags.hpp>

namespace boost { namespace network {

    template <class Tag>
        struct headers_container {
            typedef void type;
        };

    template <>
        struct headers_container<tags::default_> {
            typedef std::multimap<
                string<tags::default_>::type,
                string<tags::default_>::type
                    > type;
        };

} // namespace network

} // namespace boost

#endif // NETWORK_MESSAGE_TRAITS_HEADER_CONTAINER_HPP

