#ifndef BOOST_NETWORK_PROTOCOL_HTTP_IMPL_HTTP_ASYNC_CONNECTION_HPP_20100601
#define BOOST_NETWORK_PROTOCOL_HTTP_IMPL_HTTP_ASYNC_CONNECTION_HPP_20100601

// Copyright 2010 (C) Dean Michael Berris
// Copyright 2010 (C) Sinefunc, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/thread/future.hpp>
#include <boost/throw_exception.hpp>
#include <boost/cstdint.hpp>

namespace boost { namespace network { namespace http { namespace impl {

    template <class Tag, unsigned version_major, unsigned version_minor>
    struct async_connection_base;

    template <class Tag, unsigned version_major, unsigned version_minor>
    struct http_async_connection
        : async_connection_base<Tag,version_major,version_minor>,
        boost::enable_shared_from_this<http_async_connection<Tag,version_major,version_minor> >
    {
            typedef async_connection_base<Tag,version_major,version_minor> base;
            typedef typename base::resolver_type resolver_type;
            typedef typename base::resolver_base::resolver_iterator resolver_iterator;
            typedef typename base::resolver_base::resolver_iterator_pair resolver_iterator_pair;
            typedef typename base::response response;
            typedef typename base::string_type string_type;
            typedef typename base::request request;
            typedef typename base::resolver_base::resolve_function resolve_function;

            http_async_connection(
                boost::shared_ptr<resolver_type> resolver,
                resolve_function resolve, 
                bool follow_redirect 
                ) : resolver_(resolver),
                resolve_(resolve),
                follow_redirect_(follow_redirect),
                request_strand_(new boost::asio::io_service::strand(resolver->get_io_service()))
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
                    request_strand_->wrap(
                        boost::bind(
                        &http_async_connection<Tag,version_major,version_minor>::handle_resolved,
                        http_async_connection<Tag,version_major,version_minor>::shared_from_this(),
                        port(request),
                        _1, _2)));
                
                return temp;
            }

    private:
        void handle_resolved(boost::uint16_t port, boost::system::error_code const & ec, resolver_iterator_pair endpoint_range) {
            resolver_iterator iter = boost::begin(endpoint_range);
            if (!ec && !boost::empty(endpoint_range)) {
                boost::asio::ip::tcp::endpoint endpoint(
                    iter->endpoint().address(),
                    port
                    );
                socket_.reset(new boost::asio::ip::tcp::socket(
                    resolver_->get_io_service()));
                socket_->async_connect(
                    endpoint,
                    request_strand_->wrap(
                        boost::bind(
                            &http_async_connection<Tag,version_major,version_minor>::handle_connected,
                            http_async_connection<Tag,version_major,version_minor>::shared_from_this(),
                            std::make_pair(++iter, resolver_iterator()),
                            boost::asio::placeholders::error
                            )));
            } else {
                boost::system::system_error error(
                        ec ? ec : boost::asio::error::host_not_found
                        );
                version_promise.set_exception(boost::copy_exception(error));
                status_promise.set_exception(boost::copy_exception(error));
                status_message_promise.set_exception(boost::copy_exception(error));
                headers_promise.set_exception(boost::copy_exception(error));
                source_promise.set_exception(boost::copy_exception(error));
                destination_promise.set_exception(boost::copy_exception(error));
                body_promise.set_exception(boost::copy_exception(error));
            }
        }

        void handle_connected(resolver_iterator_pair endpoint_range, boost::system::error_code const & ec) {
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

        boost::shared_ptr<resolver_type> resolver_;
        resolve_function resolve_;
        bool follow_redirect_;
        boost::shared_ptr<boost::asio::io_service::strand> request_strand_;
        boost::shared_ptr<boost::asio::ip::tcp::socket> socket_;
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