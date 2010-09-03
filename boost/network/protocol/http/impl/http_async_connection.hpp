#ifndef BOOST_NETWORK_PROTOCOL_HTTP_IMPL_HTTP_ASYNC_CONNECTION_HPP_20100601
#define BOOST_NETWORK_PROTOCOL_HTTP_IMPL_HTTP_ASYNC_CONNECTION_HPP_20100601

// Copyright 2010 (C) Dean Michael Berris
// Copyright 2010 (C) Sinefunc, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/version.hpp>
#include <boost/thread/future.hpp>
#include <boost/throw_exception.hpp>
#include <boost/cstdint.hpp>
#include <boost/range/algorithm/transform.hpp>
#include <boost/network/constants.hpp>
#include <boost/network/traits/ostream_iterator.hpp>
#include <boost/regex.hpp>
#include <iterator>

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

            struct to_http_headers {
                typedef typename string<Tag>::type string_type;
                template <class U>
                string_type const operator() (U const & pair) const {
                    typedef typename ostringstream<Tag>::type ostringstream_type;
                    ostringstream_type header_line;
                    header_line << pair.first
                        << constants<Tag>::colon()
                        << pair.second
                        << constants<Tag>::crlf();
                    return header_line.str();
                }
            };

            virtual response start(request const & request, string_type const & method, bool get_body) {
                response temp;
                boost::shared_future<string_type> source_future = source_promise.get_future();
                source(temp, source_future);
                boost::shared_future<string_type> destination_future = destination_promise.get_future();
                destination(temp, destination_future);
                boost::shared_future<typename headers_container<Tag>::type> headers_future = headers_promise.get_future();
                headers(temp, headers_future);
                boost::shared_future<string_type> body_future = body_promise.get_future();
                body(temp, body_future);
                boost::shared_future<string_type> version_future = version_promise.get_future();
                version(temp, version_future);
                boost::shared_future<boost::uint16_t> status_future = status_promise.get_future();
                status(temp, status_future);
                boost::shared_future<string_type> status_message_future = status_message_promise.get_future();
                status_message(temp, status_message_future);

                if (!get_body) body_promise.set_value(string_type());

                typename ostringstream<Tag>::type command_stream;
                string_type uri_str = uri(request);
                typedef constants<Tag> constants;
                command_stream 
                    << method << constants::space()
                    << uri_str << constants::space()
                    << constants::http_slash() << version_major
                    << constants::dot() << version_minor
                    << constants::crlf();
                
                typedef typename headers_range<typename base::request>::type headers_range_type;
                headers_range_type headers_ = headers(request);
                boost::range::transform(
                    headers_,
                    typename ostream_iterator<Tag, string_type>::type (command_stream),
                    to_http_headers());

                if (boost::empty(headers(request)[constants::host()])) {
                    string_type host_str = host(request);
                    command_stream
                        << constants::host() << constants::colon() << constants::space() << host_str << constants::crlf();
                }

                if (boost::empty(headers(request)[constants::accept()])) {
                    command_stream
                        << constants::accept() << constants::colon() << constants::space() << constants::default_accept_mime() << constants::crlf();
                }

                if (version_major == 1u && version_minor == 1u && boost::empty(headers(request)[constants::accept_encoding()])) {
                    command_stream
                        << constants::accept_encoding() << constants::colon() << constants::space() << constants::default_accept_encoding() << constants::crlf();
                }

                if (boost::empty(headers(request)[constants::user_agent()])) {
                    command_stream
                        << constants::user_agent() << constants::colon() << constants::space() << constants::cpp_netlib_slash() << BOOST_NETLIB_VERSION << constants::crlf();
                }

                command_string_ = command_stream.str();
                
                resolve_(resolver_, host(request),
                    request_strand_->wrap(
                        boost::bind(
                        &http_async_connection<Tag,version_major,version_minor>::handle_resolved,
                        http_async_connection<Tag,version_major,version_minor>::shared_from_this(),
                        method, port(request),
                        _1, _2)));
                
                return temp;
            }

    private:
        void handle_resolved(string_type method, boost::uint16_t port, boost::system::error_code const & ec, resolver_iterator_pair endpoint_range) {
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
                            method, port, std::make_pair(++iter, resolver_iterator()),
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

        void handle_connected(string_type method, boost::uint16_t port, resolver_iterator_pair endpoint_range, boost::system::error_code const & ec) {
            if (!ec) {
                boost::asio::async_write(*socket_, boost::asio::buffer(command_string_.data(), command_string_.size()),
                    request_strand_->wrap(
                        boost::bind(
                            &http_async_connection<Tag,version_major,version_minor>::handle_sent_request,
                            http_async_connection<Tag,version_major,version_minor>::shared_from_this(),
                            method,
                            boost::asio::placeholders::error,
                            boost::asio::placeholders::bytes_transferred
                            )));
            } else {
                if (!boost::empty(endpoint_range)) {
                    resolver_iterator iter = boost::begin(endpoint_range);
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
                                method, port, std::make_pair(++iter, resolver_iterator()),
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
        }

        void handle_sent_request(string_type method, boost::system::error_code const & ec, std::size_t bytes_transferred) {
            if (!ec) {
                static boost::regex const version_pattern("HTTP/\\d+\\.\\d+\\s\\d{3}\\s[\\w\\s\\d]+\\r\\n");
                boost::asio::async_read_until(
                    *socket_,
                    response_buffer_,
                    version_pattern,
                    request_strand_->wrap(
                        boost::bind(
                            &http_async_connection<Tag,version_major,version_minor>::handle_received_status,
                            http_async_connection<Tag,version_major,version_minor>::shared_from_this(),
                            method, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)));
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

        void handle_received_status(string_type method, boost::system::error_code const & ec, std::size_t bytes_transferred) {
            if (!ec) {
                //FIXME -- make it happen
            }
        }

        void handle_received_headers() {
            //FIXME -- wire the correct parameters
        }

        void handle_recieved_body() {
            //FIXME -- wire the correct parameters
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
        string_type command_string_;
        boost::asio::streambuf response_buffer_;
    };

} // namespace impl

} // namespace http

} // namespace network

} // namespace boost

#endif // BOOST_NETWORK_PROTOCOL_HTTP_IMPL_HTTP_ASYNC_CONNECTION_HPP_20100601
