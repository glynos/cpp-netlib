#ifndef BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_DIRECTIVES_STATUS_MESSAGE_HPP_20100603
#define BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_DIRECTIVES_STATUS_MESSAGE_HPP_20100603

// Copyright 2010 (c) Dean Michael Berris
// Copyright 2010 (c) Sinefunc, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/tags.hpp>

namespace boost { namespace network { namespace http {

    template <class Tag>
    struct basic_response;

    template <class Tag>
    struct status_message_directive {

        typedef typename string<Tag>::type string_type;

        mutable string_type status_message_;

        status_message_directive(string_type const & status_message)
            : status_message_(status_message) {}

        status_message_directive(status_message_directive const & other)
            : status_message(other.status_message_) {}

        template <class T> basic_response<T> const & operator() (basic_response<T> const & response) const {
            response.status_message(status_message_);
            return response;
        }

    };

    inline status_message_directive<tags::default_> const status_message(string<tags::default_>::type const & status_message_) {
        return status_message_directive<tags::default_>(status_message_);
    }

} // namespace http

} // namespace network 

} // namespace boost

#endif // BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_DIRECTIVES_STATUS_MESSAGE_HPP_20100603