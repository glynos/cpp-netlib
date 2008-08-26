
//          Copyright Dean Michael Berris 2008.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_TAGS_HPP
#define BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_TAGS_HPP

#include <string>

namespace boost { namespace network { namespace http {

    // The HTTP message tag type
    // also defines the default character type
    // and string type(s) through a set of traits
    // classes
    //
    struct message_tag {
    };

    template <class Tag>
        struct char_traits;

    template <class Tag>
        struct string_traits;

    template <class Tag>
        struct ostringstream_traits;

    template <>
        struct char_traits<message_tag> {
            typedef char type;
        };

    template <>
        struct string_traits<message_tag> {
            typedef std::string type;
        };

    template <>
        struct ostringstream_traits<message_tag> {
            typedef std::ostringstream type;
        };

} // namespace http

} // namespace network

} // namespace boost

#endif // BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_TAGS_HPP

