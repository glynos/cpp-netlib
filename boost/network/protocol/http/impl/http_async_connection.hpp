#ifndef BOOST_NETWORK_PROTOCOL_HTTP_IMPL_HTTP_ASYNC_CONNECTION_HPP_20100601
#define BOOST_NETWORK_PROTOCOL_HTTP_IMPL_HTTP_ASYNC_CONNECTION_HPP_20100601

// Copyright 2010 (C) Dean Michael Berris
// Copyright 2010 (C) Sinefunc, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/thread/future.hpp>
#include <boost/cstdint.hpp>

namespace boost { namespace network { namespace http { namespace impl {

    template <class Tag, unsigned version_major, unsigned version_minor>
    struct async_connection_base;

    template <class Tag, unsigned version_major, unsigned version_minor>
    struct http_async_connection
        : async_connection_base<Tag,version_major,version_minor> {
            typedef async_connection_base<Tag,version_major,version_minor> base;
            typedef typename base::resolver_type resolver_type;
            typedef typename base::resolver_base::resolver_iterator_pair resolver_iterator_pair;
            typedef typename base::response response;
            typedef typename base::string_type string_type;
            typedef typename base::request request;

            http_async_connection(
                boost::shared_ptr<resolver_type> resolver,
                resolver_function resolve, 
                bool follow_redirect, )
                : resolver_(resolver),
                resolve_(resolve),
                follow_redirect_(follow_redirect)
            {}

            virtual response start(request const & request, string_type const & method, bool get_body) {
                response temp;
                source(temp, source_promise.get_future());
                destination(temp, destination_promise.get_future());
                headers(temp, headers_promise.get_future());
                body(temp, body_promise.get_future());
                version(temp, version_promise.get_future());
                status(temp, status_promise.get_future());
                status_message(temp, status_message_promise.get_future());

                if (!get_body) body_promise.set_value(string_type());

                // start things off by resolving the host.
                resolve_(resolver_, host(request),
                    request_strand->wrap(
                        boost::bind(
                        &http_async_connection<Tag,version_major,version_minor>::handle_resolved,
                        http_async_connection<Tag,version_major,version_minor>::shared_from_this(),
                        _1, _2)));
                
                return temp;
            }

    private:
        void handle_resolved(boost::system::error_code & ec, resolver_iterator_pair endpoint_range) {
            if (!ec) {
                async_connect(); //FIXME -- wire the correct parameters
            }
        }

        void handle_connected() {
            if (!ec) {
                async_send(); //FIXME -- wire the correct parameters
            }
        }

        void handle_sent_request() {
            if (!ec) {
                async_read(); //FIXME -- wire the correct parameters
            }
        }

        void handle_received_status() {
            if (!ec) {
                async_read(); //FIXME -- wire the correct parameters
            }
        }

        void handle_received_headers() {
            if (!ec) {
                async_read(); //FIXME -- wire the correct parameters
            }
        }

        void handle_recieved_body() {
            if (!ec) {
                async_read(); //FIXME -- wire the correct parameters
            }
        }

        resolve_function resolve;
        boost::promise<string_type> version_promise;
        boost::promise<boost::uint16_t> status_promise;
        boost::promise<string_type> status_message_promise;
        boost::promise<typename headers_container<Tag>::type> headers_promise;
        boost::promise<string_type> source_promise;
        boost::promise<string_type> destination_promise;
        boost::promise<string_type> body_promise;
    };

} // namespace impl

} // namespace http

} // namespace network

} // namespace boost

#endif // BOOST_NETWORK_PROTOCOL_HTTP_IMPL_HTTP_ASYNC_CONNECTION_HPP_20100601