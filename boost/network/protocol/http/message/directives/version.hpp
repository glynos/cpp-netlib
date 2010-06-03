#ifndef BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_DIRECTIVES_VERSION_HPP_20100603
#define BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_DIRECTIVES_VERSION_HPP_20100603

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
    struct version_directive {

        typedef typename string<Tag>::type string_type;

        mutable string_type version_;

        version_directive(string_type const & version)
            : version_(version) {}

        version_directive(version_directive const & other)
            : version_(other.version_) {}

        template <class T> basic_response<T> const & operator() (basic_response<T> const & response) const {
            response.version() = version_;
            return response;
        }
    
    };

    inline version_directive<tags::default_> const version(string<tags::default_>::type const & version_) {
        return version_directive<tags::default_>(version_);
    }

} // namespace http

} // namespace network

} // namespace boost


#endif // BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_DIRECTIVES_VERSION_HPP_20100603
