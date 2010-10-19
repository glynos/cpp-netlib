#ifndef BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_MODIFIERS_REMOVE_HEADER_HPP_20101019
#define BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_MODIFIERS_REMOVE_HEADER_HPP_20101019

// Copyright 2010 (c) Dean Michael Berris
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/protocol/http/tags.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <algorithm>

namespace boost { namespace network {

    namespace impl {
        
        template <class KeyType>
        struct equals {
            explicit equals(KeyType const & key_)
            : key(key_) {}
            equals(equals const & other)
            : key(other.key) {}
            template <class RequestHeader>
            bool operator()(RequestHeader const & header) {
                return boost::iequals(header.name, key);
            }
            KeyType const & key;
        };

        template <class Message, class KeyType, class Async>
        inline void remove_header(Message const & message, KeyType const & key, http::tags::http_server const &, Async const &) {
            typedef typename Message::headers_container_type::iterator iterator;
            iterator end = message.headers.end();
            iterator new_end = std::remove_if(
                message.headers.begin(),
                message.headers.end(),
                equals<KeyType>(key));
            message.headers.erase(new_end, end);
        }

    } /* impl */
    
} /* network */
    
} /* boost */

#include <boost/network/message/modifiers/remove_header.hpp>

#endif /* BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_MODIFIERS_REMOVE_HEADER_HPP_20101019 */
