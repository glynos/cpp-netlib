
// Copyright 2010 Dean Michael Berris. 
// Copyright 2010 Glyn Matthews.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_NETWORK_PROTOCOL_HTTP_SERVER_SYNC_SERVER_HPP_20101025
#define BOOST_NETWORK_PROTOCOL_HTTP_SERVER_SYNC_SERVER_HPP_20101025

#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/network/protocol/http/response.hpp>
#include <boost/network/protocol/http/request.hpp>
#include <boost/network/protocol/http/server/sync_connection.hpp>
#include <boost/network/protocol/http/server/parameters.hpp>
#include <boost/network/protocol/http/server/storage_base.hpp>
#include <boost/network/traits/string.hpp>
#include <boost/thread/mutex.hpp>

namespace boost { namespace network { namespace http {

    struct socket_options_base {
    protected:
        asio::socket_base::reuse_address acceptor_reuse_address;
        asio::socket_base::enable_connection_aborted acceptor_report_aborted;
        asio::socket_base::receive_buffer_size receive_buffer_size;
        asio::socket_base::send_buffer_size send_buffer_size;
        asio::socket_base::receive_low_watermark receive_low_watermark;
        asio::socket_base::send_low_watermark send_low_watermark;
        asio::socket_base::non_blocking_io non_blocking_io;
        asio::socket_base::linger linger;

        template <class ArgPack>
        socket_options_base(ArgPack const & args)
        : acceptor_reuse_address(args[_reuse_address|false])
        , acceptor_report_aborted(args[_report_aborted|false])
        , receive_buffer_size(args[_receive_buffer_size|1024])
        , send_buffer_size(args[_send_buffer_size|1024])
        , receive_low_watermark(args[_receive_low_watermark|512])
        , send_low_watermark(args[_send_low_watermark|512])
        , non_blocking_io(args[_non_blocking_io|true])
        , linger(args[_linger|true], args[_linger_timeout|0])
        {}

        void acceptor_options(boost::asio::ip::tcp::acceptor & acceptor) {
            acceptor.set_option(acceptor_reuse_address);
            acceptor.set_option(acceptor_report_aborted);
        }

        void socket_options(boost::asio::ip::tcp::socket & socket) {
            socket.set_option(receive_buffer_size);
            socket.set_option(receive_low_watermark);
            socket.set_option(send_buffer_size);
            socket.set_option(send_low_watermark);
            socket.io_control(non_blocking_io);
            socket.set_option(linger);
        }
    };

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
            acceptor_.cancel();
        }

        void listen() {
            boost::unique_lock<boost::mutex> listening_lock(listening_mutex_);
            if (!listening_) start_listening();
            listening_lock.unlock();
        }

        private:

        Handler & handler_;
        string_type address_, port_;
        boost::asio::ip::tcp::acceptor acceptor_;
        boost::shared_ptr<sync_connection<Tag,Handler> > new_connection;
        boost::mutex listening_mutex_;
        bool listening_;

        void handle_accept(boost::system::error_code const & ec) {
            if (!ec) {
                socket_options_base::socket_options(new_connection->socket());
                new_connection->start();
                new_connection.reset(new sync_connection<Tag,Handler>(service_, handler_));
                acceptor_.async_accept(new_connection->socket(),
                    boost::bind(&sync_server_base<Tag,Handler>::handle_accept,
                                this, boost::asio::placeholders::error));
            }
        }

        void start_listening() {
            using boost::asio::ip::tcp;
            tcp::resolver resolver(service_);
            tcp::resolver::query query(address_, port_);
            tcp::endpoint endpoint = *resolver.resolve(query);
            acceptor_.open(endpoint.protocol());
            acceptor_.bind(endpoint);
            socket_options_base::acceptor_options(acceptor_);
            acceptor_.listen();
            listening_ = true;
            new_connection.reset(new sync_connection<Tag,Handler>(service_, handler_));
            acceptor_.async_accept(new_connection->socket(),
                boost::bind(&sync_server_base<Tag,Handler>::handle_accept,
                            this, boost::asio::placeholders::error));
        }
    };

} /* http */
    
} /* network */
    
} /* boost */

#endif /* BOOST_NETWORK_PROTOCOL_HTTP_SERVER_SYNC_SERVER_HPP_20101025 */
