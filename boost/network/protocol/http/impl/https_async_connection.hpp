#ifndef BOOST_NETWORK_PROTOCOL_HTTP_IMPL_HTTPS_ASYNC_CONNECTION_HPP_20100601
#define BOOST_NETWORK_PROTOCOL_HTTP_IMPL_HTTPS_ASYNC_CONNECTION_HPP_20100601

// Copyright 2010 (C) Dean Michael Berris
// Copyright 2010 (C) Sinefunc, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

namespace boost { namespace network { namespace http { namespace impl {

    template <class Tag, unsigned version_major, unsigned version_minor>
    struct async_connection_base;

    template <class Tag, unsigned version_major, unsigned version_minor>
    struct https_async_connection
        : async_connection_base<Tag,version_major,version_minor>,
        boost::enable_shared_from_this<https_async_connection<Tag,version_major,version_minor> >
    {
            typedef async_connection_base<Tag,version_major,version_minor> base;
            typedef typename base::resolver_type resolver_type;
            typedef typename base::string_type string_type;
            typedef typename base::response response;
            typedef typename base::request request;

            https_async_connection(resolver_type & resolver)
                : resolver_(resolver) {}

            virtual response start(string_type const & hostname, string_type const & port, request const & request) {
                response temp;

                return temp;
            }

    private:
        void handle_resolved();
        void handle_connected();
        void handle_sent_request();
        void handle_received_status();
        void handle_received_headers();
        void handle_received_body();

        resolver_type & resolver_;
    };
    
} // namespace impl

} // namespace http

} // namespace network

} // namespace boost

#endif // BOOST_NETWORK_PROTOCOL_HTTP_IMPL_HTTPS_ASYNC_CONNECTION_HPP_20100601
