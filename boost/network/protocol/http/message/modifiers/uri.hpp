#ifndef BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_MODIFIERS_URI_HPP_20100621
#define BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_MODIFIERS_URI_HPP_20100621

// Copyright 2010 (c) Dean Michael Berris
// Copyright 2010 (c) Sinefunc, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/support/is_async.hpp>
#include <boost/thread/future.hpp>

namespace boost { namespace network { namespace http {

    template <class Tag>
    class basic_request;

    namespace impl {

        template <class Tag, class T>
        void uri(basic_request<Tag> & response, T const & value, mpl::false_ const &) {
            response << boost::network::http::uri(value);
        }

        template <class Tag, class T>
        void uri(basic_request<Tag> & response, boost::shared_future<T> future, mpl::true_ const &) {
            response.uri() = future;
        }

    } // namespace impl

    template <class Tag, class T>
    void uri(basic_request<Tag> & response, T const & value) {
        impl::uri(response, value, is_async<Tag>());
    }

} // namespace http

} // namespace network

} // namespace boost

#endif // BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_MODIFIERS_URI_HPP_20100621
