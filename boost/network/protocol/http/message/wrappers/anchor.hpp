#ifndef BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_WRAPPERS_ANCHOR_HPP_20100618
#define BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_WRAPPERS_ANCHOR_HPP_20100618

// Copyright 2010 (c) Dean Michael Berris.
// Copyright 2010 (c) Sinefunc, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/protocol/http/request_concept.hpp>
#include <boost/concept/requires.hpp>

namespace boost { namespace network { namespace http {

    template <class Tag>
    class basic_request;

    namespace impl {
        template <class Tag>
        struct anchor_wrapper {
            basic_request<Tag> const & message_;
            anchor_wrapper(basic_request<Tag> const & message)
                : message_(message) {}
            typedef typename basic_request<Tag>::string_type string_type;
            operator string_type() {
                return message_.anchor();
            }
        };
    }

    template <class Tag> inline
    BOOST_CONCEPT_REQUIRES(((Request<basic_request<Tag> >)),
        (impl::anchor_wrapper<Tag>))
    anchor(basic_request<Tag> const & request) {
        return impl::anchor_wrapper<Tag>(request);
    }

} // namespace http

} // namespace network

} // nmaespace boost

#endif // BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_WRAPPERS_ANCHOR_HPP_20100618
