// This file is part of the Boost Network library
// Based on the Pion Network Library (r421)
// Copyright Atomic Labs, Inc. 2007-2008
// See http://cpp-netlib.sourceforge.net for library home page.
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
// Some changes Copyright (c) Dean Michael Berris 2008

#ifndef BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_HPP
#define BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_HPP

#include <boost/network/protocol/http/tags.hpp>
#include <boost/network/protocol/http/traits.hpp>
#include <boost/network/message.hpp>
#include <string>

namespace boost { namespace network { namespace http {

    /// base class for HTTP messages (requests and responses)
    template <typename Tag>
    struct message_impl : public basic_message<Tag> {

            typedef typename string_traits<Tag>::type string_type;

            /// escapes URL-encoded strings (a%20value+with%20spaces)
            static string_type const url_decode(string_type const & str);
        
            /// encodes strings so that they are safe for URLs (with%20spaces)
            static string_type const url_encode(string_type const & str);
        
            /// builds an HTTP query string from a collection of query parameters
            static string_type const make_query_string(typename query_container<Tag>::type const & query_params);
            
            /**
             * creates a "Set-Cookie" header
             *
             * @param name the name of the cookie
             * @param value the value of the cookie
             * @param path the path of the cookie
             * @param has_max_age true if the max_age value should be set
             * @param max_age the life of the cookie, in seconds (0 = discard)
             *
             * @return the new "Set-Cookie" header
             */
            static string_type const make_set_cookie_header(string_type const & name,
                                                      string_type const & value,
                                                      string_type const & path,
                                                      bool const has_max_age = false,
                                                      unsigned long const max_age = 0);	

            /** decodes base64-encoded strings
             *
             * @param input base64 encoded string
             * @param output decoded string ( may include non-text chars)
             * @return true if successful, false if input string contains non-base64 symbols
             */
            static bool const base64_decode(string_type const &input, string_type & output);
        
            /** encodes strings using base64
             *
             * @param input arbitrary string ( may include non-text chars)
             * @param output base64 encoded string
             * @return true if successful
             */
            static bool const base64_encode(string_type const &input, string_type & output);
    };

    typedef message_impl<http::message_tag> message;

} // namespace http

} // namespace network 

} // namespace boost

// import implementation file
#include <boost/network/protocol/http/impl/message.ipp>

#endif // BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_HPP
