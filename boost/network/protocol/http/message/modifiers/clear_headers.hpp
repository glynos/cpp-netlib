#ifndef BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_MODIFIERS_CLEAR_HEADERS_HPP_20101019
#define BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_MODIFIERS_CLEAR_HEADERS_HPP_20101019

// Copyright 2010 (c) Dean Michael Berris
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/protocol/http/tags.hpp>

namespace boost { namespace network {

    namespace impl {

        template <class Message, class Async>
        inline void clear_headers(Message const & message, http::tags::http_server const &, Async const &) {
            (typename Message::headers_container_type()).swap(message.headers);
        }
        
    } /* impl */
    
} /* network */
    
} /* boost */

#include <boost/network/message/modifiers/clear_headers.hpp>

#endif /* BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_MODIFIERS_CLEAR_HEADERS_HPP_20101019 */
