#ifndef BOOST_NETWORK_PROTOCOL_HTTP_SERVER_CONNECTION_HPP_20101027
#define BOOST_NETWORK_PROTOCOL_HTTP_SERVER_CONNECTION_HPP_20101027

// Copyright 2010 Dean Michael Berris.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

namespace boost { namespace network { namespace http {

    template <class Tag, class Handler>
    struct async_connection {
        enum status_t {
            ok = 200
            , method_not_supported = 306
        };
        template <class Range>
        void set_headers(Range);
        void set_status(status_t);
        template <class Range>
        void write(Range);
        void flush();
        void close();
    };
    
} /* http */
    
} /* network */
    
} /* boost */

#endif /* BOOST_NETWORK_PROTOCOL_HTTP_SERVER_CONNECTION_HPP_20101027 */
