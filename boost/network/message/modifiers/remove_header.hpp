
#ifndef BOOST_NETWORK_MESSAGE_MODIFIER_REMOVE_HEADER_HPP_20100824
#define BOOST_NETWORK_MESSAGE_MODIFIER_REMOVE_HEADER_HPP_20100824

// Copyright 2010 (c) Dean Michael Berris
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/support/is_async.hpp>
#include <algorithm>

namespace boost { namespace network {

    namespace impl {

        template <class Message, class KeyType>
        inline void remove_header(Message const & message, KeyType const & key, tags::default_string const &, mpl::false_ const &) {
            message.headers().erase(key);
        }

        template <class Message, class KeyType>
        inline void remove_header(Message const & message, KeyType const & key, tags::default_wstring const &, mpl::false_ const &) {
            message.headers().erase(key);
        }

        template <class Message, class KeyType, class Tag>
        inline void remove_header(Message const & message, KeyType const & key, Tag const &, mpl::true_ const &) {
            message.remove_header(key);
        }

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
        inline void remove_header(Message const & message, KeyType const & key, tags::http_server const &, Async const &) {
            typedef typename Message::headers_container_type::iterator iterator;
            iterator end = message.headers.end();
            iterator new_end = std::remove_if(
                message.headers.begin(),
                message.headers.end(),
                equals<KeyType>(key));
            message.headers.erase(new_end, end);
        }

    } // namespace impl

    template <class Tag, template <class> class Message, class KeyType>
    inline void remove_header(Message<Tag> const & message, KeyType const & key) {
        impl::remove_header(message, key, Tag(), is_async<Tag>());
    }

} // namespace network

} // namespace boost

#endif // BOOST_NETWORK_MESSAGE_MODIFIER_REMOVE_HEADER_HPP_20100824
