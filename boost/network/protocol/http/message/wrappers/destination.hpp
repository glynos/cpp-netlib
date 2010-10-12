#ifndef BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_DESTINATION_HPP_20100624
#define BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_DESTINATION_HPP_20100624

// Copyright 2010 (c) Dean Michael Berris.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/protocol/http/response_concept.hpp>
#include <boost/network/protocol/http/request_concept.hpp>
#include <boost/concept/requires.hpp>

namespace boost { namespace network { namespace http {

    template <class Tag>
    struct basic_response;

    template <class Tag>
    struct basic_request;

    namespace impl {

        template <class Message>
        struct destination_wrapper {
            typedef typename string<typename Message::tag>::type string_type;
            Message const & message_;
            destination_wrapper(Message const & message)
                : message_(message) {}
            destination_wrapper(destination_wrapper const & other)
                : message_(other.message_) {}
            operator string_type const () {
                return message_.source();
            }
        };

    } // namespace impl

    template <class Tag>
    inline
    BOOST_CONCEPT_REQUIRES(((Response<basic_response<Tag> >)),
        (impl::destination_wrapper<basic_response<Tag> > const))
    destination(basic_response<Tag> const & message) {
        return impl::destination_wrapper<basic_response<Tag> >(message);
    }

    template <class Tag>
    inline
    BOOST_CONCEPT_REQUIRES(((Request<basic_request<Tag> >)),
        (impl::destination_wrapper<basic_request<Tag> > const))
    destination(basic_request<Tag> const & message) {
        return impl::destination_wrapper<basic_request<Tag> >(message);
    }

} // namespace http

} // namespace network

} // namespace boost

#endif // BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_DESTINATION_HPP_20100624
