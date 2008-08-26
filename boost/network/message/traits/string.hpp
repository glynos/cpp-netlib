
//          Copyright Dean Michael Berris 2008.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef NETWORK_MESSAGE_TRAITS_STRING_HPP
#define NETWORK_MESSAGE_TRAITS_STRING_HPP

#include <string>
#include <boost/network/message/tags.hpp>

namespace boost { namespace network {

    template <class Tag>
        struct string { 
            typedef void type;
        };

    template <>
        struct string<tags::default_> {
            typedef std::string type;
        };

} // namespace network

} // namespace boost

#endif // NETWORK_MESSAGE_TRAITS_STRING_HPP

