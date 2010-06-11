#ifndef BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_BASE_HPP_20100603
#define BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_BASE_HPP_20100603

// Copyright 2010 (c) Dean Michael Berris
// Copyright 2010 (c) Sinefunc, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/tags.hpp>

#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_base_of.hpp>

namespace boost { namespace network { namespace http {

    namespace impl {
        template <class Tag>
        struct async_message_base {};
    };

    template <class Tag>
    struct message_base
        : mpl::if_<
            is_base_of<
                tags::async,
                Tag
            >,
            impl::async_message_base<Tag>,
            message_impl<Tag>
        >
    {};

} // namespace http

} // namespace network

} // namespace boost

#endif // BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_BASE_HPP_20100603
