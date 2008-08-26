
//          Copyright Dean Michael Berris 2008.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_TRAITS_HEADERS_CONTAINER_IPP
#define BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_TRAITS_HEADERS_CONTAINER_IPP

#include <boost/network/message/traits/headers_container.hpp>
#include <boost/network/protocol/http/tags.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <map>
#include <string>

namespace boost { namespace network {

    template <>
        struct headers_container<http::message_tag> {

            // Moving implementation from original
            // message_traits implementation by
            // Atomic Labs, Inc.
            // --
            // returns true if str1 < str2 (ignoring case)
            struct is_less_ignore_case {
                inline bool operator() (
                        http::string_traits<http::message_tag>::type const & str1, 
                        http::string_traits<http::message_tag>::type const & str2) const {
                    return to_lower_copy(str1) < to_lower_copy(str2);
                };
            };

            typedef std::multimap<
                http::string_traits<http::message_tag>::type,
                http::string_traits<http::message_tag>::type,
                is_less_ignore_case> type;
        };

} // namespace network

} // namespace boost

#endif // BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_TRAITS_HEADERS_CONTAINER_IPP

