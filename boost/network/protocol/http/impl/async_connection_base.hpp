#ifndef BOOST_NETWORK_PROTOCOL_HTTP_IMPL_ASYNC_CONNECTION_BASE_20100529
#define BOOST_NETWORK_PROTOCOL_HTTP_IMPL_ASYNC_CONNECTION_BASE_20100529

// Copyright 2010 (C) Dean Michael Berris
// Copyright 2010 (C) Sinefunc, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/protocol/http/response.hpp>
#include <boost/network/protocol/http/impl/http_async_connection.hpp>
#include <boost/network/protocol/http/impl/https_async_connection.hpp>

namespace boost { namespace network { namespace http { namespace impl {

    template <class Tag, unsigned version_major, unsigned version_minor>
    struct async_connection_base {
        typedef typename resolver_policy<Tag>::type resolver_base;
        typedef typename resolver_base::resolver_type resolver_type;
        typedef typename string<Tag>::type string_type;
        typedef basic_request<Tag> request;
        typedef basic_response<Tag> response;
        
        static async_connection_base<Tag,version_major,version_minor> * new_connection(resolver_type & resolver, bool https) {
            if (https) {
                return dynamic_cast<async_connection_base<Tag,version_major,version_minor>*>(new https_async_connection<Tag,version_major,version_minor>(resolver));
            }
            return dynamic_cast<async_connection_base<Tag,version_major,version_minor>*>(new http_async_connection<Tag,version_major,version_minor>(resolver));
        }

        virtual response start(string_type const & hostname, string_type const & port, request const & request) = 0;

    };

} // namespace impl

} // namespace http

} // namespace network

} // namespace boost

#endif // BOOST_NETWORK_PROTOCOL_HTTP_IMPL_ASYNC_CONNECTION_BASE_20100529
