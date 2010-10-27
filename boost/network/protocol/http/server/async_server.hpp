#ifndef BOOST_NETWORK_PROTOCOL_HTTP_SERVER_ASYNC_SERVER_HPP_20101025
#define BOOST_NETWORK_PROTOCOL_HTTP_SERVER_ASYNC_SERVER_HPP_20101025

// Copyright 2010 Dean Michael Berris. 
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/protocol/http/server/async_connection.hpp>
#include <boost/network/protocol/http/server/header.hpp>
#include <boost/network/utils/thread_pool.hpp>

namespace boost { namespace network { namespace http {
    
    template <class Tag, class Handler>
    struct async_server_base {
        typedef basic_request<Tag> request;
        typedef basic_response<Tag> response;
        typedef async_connection<Tag, Handler> connection;
        typedef response_header<Tag> response_header;
        typedef typename string<Tag>::type string_type;

        async_server_base(string_type const & address
                        , string_type const & port
                        , Handler & handler
                        , utils::thread_pool & thread_pool)
        {}

        void run() {};

    };

} /* http */
    
} /* network */
    
} /* boost */

#endif /* BOOST_NETWORK_PROTOCOL_HTTP_SERVER_ASYNC_SERVER_HPP_20101025 */
