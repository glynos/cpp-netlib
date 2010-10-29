
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
#include <boost/network/protocol/http/connection.hpp>
#include <boost/network/traits/string.hpp>

namespace boost { namespace network { namespace http {
    
    template <class Tag, class Handler>
    struct sync_server_base {
        typedef typename string<Tag>::type string_type;
        typedef basic_request<Tag> request;
        typedef basic_response<Tag> response;

        sync_server_base(string_type const & address,
                     string_type const & port,
                     Handler & handler)
        : handler_(handler)
        , service_()
        , acceptor_(service_)
        , new_connection(new connection<Tag,Handler>(service_, handler))
        {
            using boost::asio::ip::tcp;
            tcp::resolver resolver(service_);
            tcp::resolver::query query(address, port);
            tcp::endpoint endpoint = *resolver.resolve(query);
            acceptor_.open(endpoint.protocol());
            acceptor_.bind(endpoint);
            acceptor_.listen();
            acceptor_.async_accept(new_connection->socket(),
                boost::bind(&sync_server_base<Tag,Handler>::handle_accept,
                            this, boost::asio::placeholders::error));
        }

        void run() {
            service_.run();
        }

        void stop() {
            // stop accepting new connections and let all the existing handlers finish.
            acceptor_.cancel();
        }

        private:

        Handler & handler_;
        boost::asio::io_service service_;
        boost::asio::ip::tcp::acceptor acceptor_;
        boost::shared_ptr<connection<Tag,Handler> > new_connection;

        void handle_accept(boost::system::error_code const & ec) {
            if (!ec) {
                new_connection->start();
                new_connection.reset(new connection<Tag,Handler>(service_, handler_));
                acceptor_.async_accept(new_connection->socket(),
                    boost::bind(&sync_server_base<Tag,Handler>::handle_accept,
                                this, boost::asio::placeholders::error));
            }
        }
    };

} /* http */
    
} /* network */
    
} /* boost */

#endif /* BOOST_NETWORK_PROTOCOL_HTTP_SERVER_SYNC_SERVER_HPP_20101025 */
