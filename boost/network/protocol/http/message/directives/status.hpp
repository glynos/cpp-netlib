#ifndef BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_DIRECTIVES_STATUS_HPP_20100603
#define BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_DIRECTIVES_STATUS_HPP_20100603

// Copyright 2010 (c) Dean Michael Berris
// Copyright 2010 (c) Sinefunc, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/tags.hpp>
#include <boost/cstdint.hpp>

namespace boost { namespace network { namespace http {

    template <class Tag>
    struct basic_response;

    template <class Tag>
    struct status_directive {

        typedef typename string<Tag>::type string_type;

        mutable boost::uint16_t status_;

        status_directive(boost::uint16_t status)
            : status_(status) {}

        status_directive(status_directive const & other)
            : status_(other.status_) {}

        template <class T> basic_response<T> const & operator() (basic_response<T> const & response) const {
            response.status(status_);
            return response;
        }

    };

    inline status_directive<tags::default_> const status(boost::uint16_t status_) {
        return status_directive<tags::default_>(status_);
    }

} // namespace http

} // namespace network

} // namespace boost

#endif // BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_DIRECTIVES_STATUS_HPP_20100603
