
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
#include <boost/network/traits/string.hpp>
#include <boost/thread/mutex.hpp>

namespace boost { namespace network { namespace http {
    
    template <class Tag, class Handler>
    struct sync_server_base {
        typedef typename string<Tag>::type string_type;
        typedef basic_request<Tag> request;
        typedef basic_response<Tag> response;
        typedef typename boost::network::http::response_header<Tag>::type response_header;

        sync_server_base(string_type const & address,
                     string_type const & port,
                     Handler & handler)
        : handler_(handler)
        , address_(address)
        , port_(port)
        , self_service_(new boost::asio::io_service())
        , service_(*self_service_)
        , acceptor_(service_)
        , new_connection()
        , listening_mutex_()
        , listening_(false)
        {}

        void run() {
            boost::unique_lock<boost::mutex> listening_lock(listening_mutex_);
            if (!listening_) start_listening();
            listening_lock.unlock();
            service_.run();
        }

        void stop() {
            // stop accepting new connections and let all the existing handlers finish.
            acceptor_.cancel();
        }

        private:

        Handler & handler_;
        string_type address_, port_;
        std::auto_ptr<boost::asio::io_service> self_service_;
        boost::asio::io_service & service_;
        boost::asio::ip::tcp::acceptor acceptor_;
        boost::shared_ptr<sync_connection<Tag,Handler> > new_connection;
        boost::mutex listening_mutex_;
        bool listening_;

        void handle_accept(boost::system::error_code const & ec) {
            if (!ec) {
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
