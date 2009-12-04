#ifndef BOOST_NETWORK_URL_HTTP_URL_CONCEPT_HPP_
#define BOOST_NETWORK_URL_HTTP_URL_CONCEPT_HPP_

// Copyright 2009 Dean Michael Berris.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/concept_check.hpp>
#include <boost/network/uri/uri_concept.hpp>

namespace boost { namespace network { namespace uri {

    template <class U>
        struct HttpURI : URI<U> {
            typedef typename U::string_type string_type;

            BOOST_CONCEPT_USAGE(HttpURI)
            {
                string_type user_info_ = user_info(uri);
                string_type host_ = host(uri);
                uint32_t port_ = port(uri);
                port_ = 0u;
                string_type path_ = path(uri);
                string_type query_ = query(uri);
                string_type fragment_ = fragment(uri);
            }

            private:
            U uri;
        };

} // namespace uri

} // namespace network

} // namespace boost

#endif


