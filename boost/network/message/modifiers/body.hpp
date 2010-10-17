#ifndef BOOST_NETWORK_MODIFIERS_BODY_HPP_20100824
#define BOOST_NETWORK_MODIFIERS_BODY_HPP_20100824

// Copyright 2010 (c) Dean Michael Berris
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/support/is_async.hpp>
#include <boost/thread/future.hpp>

namespace boost { namespace network {

    namespace impl {

        template <class Message, class ValueType, class Tag>
        inline void body(Message const & message, ValueType const & body_, Tag const &, mpl::false_ const &) {
            message.body(body_);
        }

        template <class Message, class ValueType, class Tag>
        inline void body(Message const & message, ValueType const & body_, Tag const &, mpl::true_ const &) {
            message.body(body_);
        }

        template <class Message, class ValueType, class Async>
        inline void body(Message const & message, ValueType const & body_, tags::http_server, Async) {
            message.body = body_;
        }

    } // namespace impl

    template <class Tag, template <class> class Message, class ValueType>
    inline void body(Message<Tag> const & message, ValueType const & body_) {
        impl::body(message, body_, Tag(), is_async<Tag>());
    }

} // namespace network

} // namespace boost

#endif // BOOST_NETWORK_MODIFIERS_BODY_HPP_20100824
