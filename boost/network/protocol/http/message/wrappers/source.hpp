#ifndef BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_WRAPPERS_SOURCE_HPP_20100622
#define BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_WRAPPERS_SOURCE_HPP_20100622

// Copyright 2010 (c) Dean Michael Berris
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
        struct source_wrapper {
            typedef typename string<Tag>::type string_type;
            basic_response<Tag> const & message_;
            source_wrapper(basic_response<Tag> const & message)
                : message_(message) {}
            source_wrapper(source_wrapper const & other)
                : message_(other.message_) {}
            operator string_type const () {
                return message_.source();
            }
        };

    } // namespace impl

    template <class Tag>
    inline
    BOOST_CONCEPT_REQUIRES(((Response<basic_response<Tag> >)),
        (impl::source_wrapper<Tag> const))
    source(basic_response<Tag> const & message) {
        return impl::source_wrapper<Tag>(message);
    }

} // namespace http

} // namespace network

} // namespace boost

#endif // BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_WRAPPERS_SOURCE_HPP_20100622
