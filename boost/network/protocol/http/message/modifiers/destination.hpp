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

    template <class Tag>
    struct basic_request;

    namespace impl {

        template <class Tag, class T>
        void destination(basic_response<Tag> & response, T const & value, mpl::false_ const &) {
            response << ::boost::network::destination(value);
        }

        template <class Tag, class T>
        void destination(basic_response<Tag> & response, T const & future, mpl::true_ const &) {
            response.destination(future);
        }

        template <class Tag, class T>
        void destination(basic_request<Tag> & request, T const & value, tags::server const &) {
            request.destination = value;
        }

    }

    template <class R>
    struct Response;

    template <class Tag, class T>
    inline
    BOOST_CONCEPT_REQUIRES(((Response<basic_response<Tag> >)),
        (void))
    destination(basic_response<Tag> & response, T const & value) {
        impl::destination(response, value, is_async<Tag>());
    }

    template <class R>
    struct ServerRequest;

    template <class Tag, class T>
    inline
    BOOST_CONCEPT_REQUIRES(((ServerRequest<basic_request<Tag> >)),
        (void))
    destination(basic_request<Tag> & request, T const & value) {
        impl::destination(request, value, Tag());
    }

} // namespace http

    namespace impl {
        
        template <class Message, class ValueType, class Async>
        inline void destination(Message const & message, ValueType const & destination_, http::tags::http_server, Async) {
            message.destination = destination_;
        }

    } /* impl */

} // namespace network

} // namespace boost

#include <boost/network/message/modifiers/destination.hpp>

#endif // BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_MODIFIER_DESTINATION_HPP_20100624
