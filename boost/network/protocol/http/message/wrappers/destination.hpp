#ifndef BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_DESTINATION_HPP_20100624
#define BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_DESTINATION_HPP_20100624

// Copyright 2010 (c) Dean Michael Berris.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/protocol/http/response_concept.hpp>
#include <boost/concept/requires.hpp>

namespace boost { namespace network { namespace http {

    template <class Tag>
    struct basic_response;

    namespace impl {

        template <class Tag>
        struct destination_wrapper {
            typedef typename string<Tag>::type string_type;
            basic_response<Tag> const & message_;
            destination_wrapper(basic_response<Tag> const & message)
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
        (impl::destination_wrapper<Tag> const))
    destination(basic_response<Tag> const & message) {
        return impl::destination_wrapper<Tag>(message);
    }

} // namespace http

} // namespace network

} // namespace boost

#endif // BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_DESTINATION_HPP_20100624
