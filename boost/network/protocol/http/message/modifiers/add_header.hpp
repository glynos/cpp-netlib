
#ifndef BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_MODIFIERS_ADD_HEADER_HPP_20101019
#define BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_MODIFIERS_ADD_HEADER_HPP_20101019

// Copyright 2010 (c) Dean Michael Berris
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/protocol/http/tags.hpp>

namespace boost { namespace network {

    namespace impl {

        template <class Message, class KeyType, class ValueType, class Async>
        inline void add_header(Message const & message, KeyType const & key, ValueType const & value, http::tags::server const &, Async const &) {
            typedef typename Message::headers_container_type::value_type value_type;
            value_type header_ = { key, value };
            message.headers.insert(message.headers.end(), header_);
        }

    }
    
} /* network */
    
} /* boost */

#include <boost/network/message/modifiers/add_header.hpp>
#endif /* BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_MODIFIERS_ADD_HEADER_HPP_20101019 */
