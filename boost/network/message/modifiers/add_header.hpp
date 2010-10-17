
#ifndef BOOST_NETWORK_MESSAGE_MODIFIER_ADD_HEADER_HPP_20100824
#define BOOST_NETWORK_MESSAGE_MODIFIER_ADD_HEADER_HPP_20100824

// Copyright 2010 (c) Dean Michael Berris
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/support/is_async.hpp>

namespace boost { namespace network {

    namespace impl {
        template <class Message, class KeyType, class ValueType>
        inline void add_header(Message const & message, KeyType const & key, ValueType const & value, tags::default_string const &, mpl::false_ const &) {
            message.headers().insert(std::make_pair(key, value));
        }

        template <class Message, class KeyType, class ValueType>
        inline void add_header(Message const & message, KeyType const & key, ValueType const & value, tags::default_wstring const &, mpl::false_ const &) {
            message.headers().insert(std::make_pair(key, value));
        }

        template <class Message, class KeyType, class ValueType>
        inline void add_header(Message const & message, KeyType const & key, ValueType const & value, tags::async const &, mpl::true_ const &) {
            message.add_header(std::make_pair(key, value));
        }

        template <class Message, class KeyType, class ValueType, class Async>
        inline void add_header(Message const & message, KeyType const & key, ValueType const & value, tags::http_server const &, Async const &) {
            typedef typename Message::headers_container_type::value_type value_type;
            value_type header_ = { key, value };
            message.headers.insert(message.headers.end(), header_);
        }

    }

    template <class Tag, template <class> class Message, class KeyType, class ValueType>
    inline void add_header(Message<Tag> const & message, KeyType const & key, ValueType const & value) {
        impl::add_header(message, key, value, Tag(), is_async<Tag>());
    }

} // namespace network

} // namespace boost

#endif // BOOST_NETWORK_MESSAGE_MODIFIER_ADD_HEADER_HPP_20100824
