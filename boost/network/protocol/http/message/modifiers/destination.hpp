#ifndef BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_MODIFIER_DESTINATION_HPP_20100624
#define BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_MODIFIER_DESTINATION_HPP_20100624

// Copyright 2010 (C) Dean Michael Berris
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/support/is_async.hpp>
#include <boost/thread/future.hpp>
#include <boost/concept/requires.hpp>

namespace boost { namespace network { namespace http {

    template <class Tag>
    struct basic_response;

    namespace impl {

        template <class Tag, class T>
        void destination(basic_response<Tag> & response, T const & value, mpl::false_ const &) {
            response << destination(value);
        }

        template <class Tag, class T>
        void destination(basic_response<Tag> & response, boost::shared_future<T> future, mpl::true_ const &) {
            response.destination(future);
        }

    }

    template <class Tag, class T>
    inline
    BOOST_CONCEPT_REQUIRES(((Response<basic_response<Tag> >)),
        (void))
    destination(basic_response<Tag> & response, T const & value) {
        impl::destination(response, value, is_async<Tag>());
    }

} // namespace http

} // namespace network

} // namespace boost

#endif // BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_MODIFIER_DESTINATION_HPP_20100624
