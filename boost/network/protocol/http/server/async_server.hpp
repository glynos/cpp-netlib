#ifndef BOOST_NETWORK_PROTOCOL_HTTP_SERVER_ASYNC_SERVER_HPP_20101025
#define BOOST_NETWORK_PROTOCOL_HTTP_SERVER_ASYNC_SERVER_HPP_20101025

// Copyright 2010 Dean Michael Berris. 
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/protocol/http/server/async_connection.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/network/protocol/http/server/storage_base.hpp>
#include <boost/network/protocol/http/server/socket_options_base.hpp>
#include <boost/network/utils/thread_pool.hpp>

namespace boost { namespace network { namespace http {
    
    template <class Tag, class Handler>
    struct async_server_base : server_storage_base, socket_options_base {
        typedef basic_request<Tag> request;
        typedef basic_response<Tag> response;
        typedef typename string<Tag>::type string_type;
        typedef typename boost::network::http::response_header<Tag>::type response_header;
        typedef async_connection<Tag,Handler> connection;
        typedef shared_ptr<connection> connection_ptr;

        template <class ArgPack>
        async_server_base(ArgPack const & args)
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
        , handler(args[_handler])
        , address_(args[_address])
        , port_(args[_port])
        , thread_pool(args[_thread_pool])
        , acceptor(server_storage_base::service_)
        , stopping(false)
        , new_connection()
        , listening_mutex_()
        , listening(false)
        {}

        void run() {
            listen();
            io_service.run();
        };

        void stop() {
            // stop accepting new requests and let all the existing
            // handlers finish.
            stopping = true;
            acceptor.cancel();
        }

        void listen() {
            if (listening) return;
            boost::unique_lock<boost::mutex> listening_lock(listening_mutex_);
            if (!listening) start_listening();
        }

    private:
        Handler & handler;
        string_type address_, port_;
        utils::thread_pool & thread_pool;
        std::auto_ptr<asio::io_service> self_service;
        asio::io_service io_service;
        asio::ip::tcp::acceptor acceptor;
        bool stopping;
        connection_ptr new_connection;
        boost::mutex listening_mutex_;
        bool listening;

        void handle_accept(boost::system::error_code const & ec) {
            if (!ec) {
                socket_options_base::socket_options(new_connection->socket());
                new_connection->start();
                if (!stopping) {
                    new_connection.reset(
                        new connection(
                            io_service
                            , handler
                            , thread_pool
                            )
                        );
                    acceptor.async_accept(new_connection->socket(),
                        boost::bind(
                            &async_server_base<Tag,Handler>::handle_accept
                            , this
                            , boost::asio::placeholders::error
                            )
                        );
                }
            }
        }

        void start_listening() {
            using boost::asio::ip::tcp;
            tcp::resolver resolver(io_service);
            tcp::resolver::query query(address_, port_);
            tcp::endpoint endpoint = *resolver.resolve(query);
            acceptor.open(endpoint.protocol());
            acceptor.bind(endpoint);
            socket_options_base::acceptor_options(acceptor);
            acceptor.listen();
            listening = true;
            new_connection.reset(new connection(io_service, handler, thread_pool));
            acceptor.async_accept(new_connection->socket(),
                boost::bind(
                    &async_server_base<Tag,Handler>::handle_accept
                    , this
                    , boost::asio::placeholders::error));
        }
    };

} /* http */
    
} /* network */
    
} /* boost */

#endif /* BOOST_NETWORK_PROTOCOL_HTTP_SERVER_ASYNC_SERVER_HPP_20101025 */
