// Copyright 2009 (c) Dean Michael Berris <mikhailberis@gmail.com>
// Copyright 2009 (c) Tarroo, Inc.
// Adapted from Christopher Kholhoff's Boost.Asio Example, released under
// the Boost Software License, Version 1.0. (See acccompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// =====================================================================================
// 
//       Filename:  connection.hpp
// 
//    Description:  Connection handler for the HTTP requests.
// 
//        Version:  1.1
//        Created:  Sunday, 15 November, 2009 07:46:40  PHT
// 
//         Author:  Dean Michael Berris (dmb), mikhailberis@gmail.com
// 
// =====================================================================================
//

#ifndef BOOST_NETWORK_HTTP_CONNECTION_HPP_
#define BOOST_NETWORK_HTTP_CONNECTION_HPP_

#include <boost/enable_shared_from_this.hpp>
#include <boost/network/protocol/http/request_parser.hpp>
#include <boost/network/protocol/http/request.hpp>
#include <boost/network/protocol/http/header.hpp>
#include <boost/network/protocol/http/reply.hpp>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/bind.hpp>

namespace boost { namespace network { namespace http {

    using boost::asio::io_service;
    namespace system = boost::system;
    using boost::asio::ip::tcp;
    using boost::array;
    using boost::tribool;
    using boost::tuple;
    namespace tuples = boost::tuples;
    using boost::tuples::tie;
    using boost::bind;
    using boost::to_lower_copy;

    template <class Handler>
    struct connection : boost::enable_shared_from_this<connection<Handler> > {

        connection(io_service & service, Handler & handler)
        : service_(service)
        , handler_(handler)
        , socket_(service_)
        , wrapper_(service_)
        {
            try {
                socket_.set_option(tcp::no_delay(true)); // Don't delay writing
            } catch (system::system_error & e) {
                handler_.log(e.what());
            }
        }

        tcp::socket & socket() {
            return socket_;
        }

        void start() {
            // This is HTTP so we really want to just
            // read and parse a request that's incoming
            // and then pass that request object to the
            // handler_ instance.
            //
            socket_.async_read_some(
                boost::asio::buffer(buffer_),
                wrapper_.wrap(
                    bind(
                        &connection<Handler>::handle_read_headers,
                        connection<Handler>::shared_from_this(),
                        boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred
                        )
                    )
                );
        }

        private:

        struct is_content_length {
            template <class Header>
            bool operator()(Header const & header) {
                return to_lower_copy(header.name) == "content-length";
            }
        };

        void handle_read_headers(system::error_code const &ec, size_t bytes_transferred) {
            if (!ec) {
                tribool done;
                tie(done,tuples::ignore) = parser_.parse_headers(request_, buffer_.data(), buffer_.data() + bytes_transferred);
                if (done) {
                    if (request_.method[0] == 'P') {
                        // look for the content-length header
                        std::vector<request_header>::iterator it = 
                            find_if(
                                request_.headers.begin(), 
                                request_.headers.end(),
                                is_content_length()
                                );
                        if (it == request_.headers.end()) {
                            reply_= reply::stock_reply(reply::bad_request);
                            boost::asio::async_write(
                                socket_,
                                reply_.to_buffers(),
                                wrapper_.wrap(
                                    bind(
                                        &connection<Handler>::handle_write,
                                        connection<Handler>::shared_from_this(),
                                        boost::asio::placeholders::error
                                        )
                                    )
                                );
                            return;
                        }

                        size_t content_length = 0;

                        try {
                            content_length = boost::lexical_cast<size_t>(it->value);
                        } catch (...) {
                            reply_= reply::stock_reply(reply::bad_request);
                            boost::asio::async_write(
                                socket_,
                                reply_.to_buffers(),
                                wrapper_.wrap(
                                    bind(
                                        &connection<Handler>::handle_write,
                                        connection<Handler>::shared_from_this(),
                                        boost::asio::placeholders::error
                                        )
                                    )
                                );
                            return;
                        }

                        if (content_length != 0) {
                            async_read(
                                socket_,
                                boost::asio::buffer(buffer_),
                                boost::asio::transfer_at_least(content_length),
                                wrapper_.wrap(
                                    bind(
                                        &connection<Handler>::handle_read_body_contents,
                                        connection<Handler>::shared_from_this(),
                                        boost::asio::placeholders::error,
                                        content_length,
                                        boost::asio::placeholders::bytes_transferred
                                        )
                                    )
                                );
                            return;
                        }

                        handler_(request_, reply_);
                        boost::asio::async_write(
                            socket_,
                            reply_.to_buffers(),
                            wrapper_.wrap(
                                bind(
                                    &connection<Handler>::handle_write,
                                    connection<Handler>::shared_from_this(),
                                    boost::asio::placeholders::error
                                    )
                                )
                            );
                    } else {
                        handler_(request_, reply_);
                        boost::asio::async_write(
                            socket_,
                            reply_.to_buffers(),
                            wrapper_.wrap(
                                bind(
                                    &connection<Handler>::handle_write,
                                    connection<Handler>::shared_from_this(),
                                    boost::asio::placeholders::error
                                    )
                                )
                            );
                    }
                } else if (!done) {
                    reply_= reply::stock_reply(reply::bad_request);
                    boost::asio::async_write(
                        socket_,
                        reply_.to_buffers(),
                        wrapper_.wrap(
                            bind(
                                &connection<Handler>::handle_write,
                                connection<Handler>::shared_from_this(),
                                boost::asio::placeholders::error
                                )
                            )
                        );
                } else {
                    socket_.async_read_some(
                        boost::asio::buffer(buffer_),
                        wrapper_.wrap(
                            bind(
                                &connection<Handler>::handle_read_headers,
                                connection<Handler>::shared_from_this(),
                                boost::asio::placeholders::error,
                                boost::asio::placeholders::bytes_transferred
                                )
                            )
                        );
                }
            }
            // TODO Log the error?
        }

        void handle_read_body_contents(boost::system::error_code const & ec, size_t bytes_to_read, size_t bytes_transferred) {
            if (!ec) {
                size_t difference = bytes_to_read - bytes_transferred;
                request_.body.append(buffer_.begin(), buffer_.end());
                if (difference == 0) {
                    handler_(request_, reply_);
                    boost::asio::async_write(
                        socket_,
                        reply_.to_buffers(),
                        wrapper_.wrap(
                            bind(
                                &connection<Handler>::handle_write,
                                connection<Handler>::shared_from_this(),
                                boost::asio::placeholders::error
                                )
                            )
                        );
                } else {
                    socket_.async_read_some(
                        boost::asio::buffer(buffer_),
                        wrapper_.wrap(
                            bind(
                                &connection<Handler>::handle_read_body_contents,
                                connection<Handler>::shared_from_this(),
                                boost::asio::placeholders::error,
                                difference,
                                boost::asio::placeholders::bytes_transferred
                                )
                            )
                        );
                }
            }
            // TODO Log the error?
        }

        void handle_write(boost::system::error_code const & ec) {
            if (!ec) {
                boost::system::error_code ignored_ec;
                socket_.shutdown(tcp::socket::shutdown_both, ignored_ec);
            }
        }

        io_service & service_;
        Handler & handler_;
        tcp::socket socket_;
        io_service::strand wrapper_;
        array<char,4096> buffer_;
        request_parser parser_;
        request_pod request_;
        reply reply_;
    };


} // namespace http

} // namespace network

} // namespace boost

#endif // BOOST_NETWORK_HTTP_CONNECTION_HPP_

