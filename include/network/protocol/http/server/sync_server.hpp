// Copyright 2010 Dean Michael Berris. 
// Copyright 2010 Glyn Matthews.
// Copyright 2012 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef NETWORK_PROTOCOL_HTTP_SERVER_SYNC_SERVER_HPP_20101025
#define NETWORK_PROTOCOL_HTTP_SERVER_SYNC_SERVER_HPP_20101025

#include <network/detail/debug.hpp>
#include <boost/shared_ptr.hpp>
#include <functional>
#include <asio/ip/tcp.hpp>
#include <network/protocol/http/response.hpp>
#include <network/protocol/http/request.hpp>
#include <network/protocol/http/server/sync_connection.hpp>
#include <network/protocol/http/server/parameters.hpp>
#include <network/protocol/http/server/storage_base.hpp>
#include <network/protocol/http/server/socket_options_base.hpp>
#include <network/traits/string.hpp>
#include <boost/thread/mutex.hpp>

namespace network { namespace http {

    template <class Tag, class Handler>
    struct sync_server_base : server_storage_base, socket_options_base {
        typedef typename string<Tag>::type string_type;
        typedef basic_request<Tag> request;
        typedef basic_response<Tag> response;
        typedef typename boost::network::http::response_header<Tag>::type response_header;

        template <class ArgPack>
        sync_server_base(ArgPack const & args)
        : server_storage_base(args,
            typename mpl::if_<
                is_same<
                    typename parameter::value_type<ArgPack, tag::io_service, void>::type,
                    void
                    >,
                server_storage_base::no_io_service,
                server_storage_base::has_io_service
                >::type())
        , socket_options_base(args)
        , handler_(args[_handler])
        , address_(args[_address])
        , port_(args[_port])
        , acceptor_(server_storage_base::service_)
        , new_connection()
        , listening_mutex_()
        , listening_(false)
        {}

        void run() {
            listen();
            service_.run();
        }

        void stop() {
            // stop accepting new connections and let all the existing handlers finish.
            system::error_code ignored;
            acceptor_.close(ignored);
            service_.stop();
        }

        void listen() {
            boost::unique_lock<boost::mutex> listening_lock(listening_mutex_);
            if (!listening_) start_listening();
        }

        private:

        Handler & handler_;
        string_type address_, port_;
        asio::ip::tcp::acceptor acceptor_;
        boost::shared_ptr<sync_connection<Tag,Handler> > new_connection;
        boost::mutex listening_mutex_;
        bool listening_;

        void handle_accept(asio::error_code const & ec) {
            if (!ec) {
                socket_options_base::socket_options(new_connection->socket());
                new_connection->start();
                new_connection.reset(new sync_connection<Tag,Handler>(service_, handler_));
                acceptor_.async_accept(new_connection->socket(),
                    std::bind(&sync_server_base<Tag,Handler>::handle_accept,
                                this, asio::placeholders::error));
            }
        }

        void start_listening() {
            using asio::ip::tcp;
            system::error_code error;
            tcp::resolver resolver(service_);
            tcp::resolver::query query(address_, port_);
            tcp::resolver::iterator endpoint_iterator = resolver.resolve(query, error);
            if (error) {
                NETWORK_MESSAGE("Error resolving address: " << address_ << ':' << port_);
                return;
            }
            tcp::endpoint endpoint = *endpoint_iterator;
            acceptor_.open(endpoint.protocol(), error);
            if (error) {
                NETWORK_MESSAGE("Error opening socket: " << address_ << ':' << port_ << " -- reason: '" << error << '\'');
                return;
            }
            socket_options_base::acceptor_options(acceptor_);
            acceptor_.bind(endpoint, error);
            if (error) {
                NETWORK_MESSAGE("Error binding to socket: " << address_ << ':' << port_ << " -- reason: '" << error << '\'');
                return;
            }
            acceptor_.listen(tcp::socket::max_connections, error);
            if (error) {
                NETWORK_MESSAGE("Error listening on socket: " << address_ << ':' << port_ << " -- reason: '" << error << '\'');
                return;
            }
            new_connection.reset(new sync_connection<Tag,Handler>(service_, handler_));
            acceptor_.async_accept(new_connection->socket(),
                std::bind(&sync_server_base<Tag,Handler>::handle_accept,
                            this, asio::placeholders::error));
            listening_ = true;
        }
    };

}  // namespace http
}  // namespace network
    
#endif /* NETWORK_PROTOCOL_HTTP_SERVER_SYNC_SERVER_HPP_20101025 */
