// Copyright 2009 (c) Tarro, Inc.
// Copyright 2009 (c) Dean Michael Berris <mikhailberis@gmail.com>
// Copyright 2010 (c) Glyn Matthews
// Copyright 2003-2008 (c) Chris Kholhoff
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_NETWORK_HTTP_SERVER_HPP_
#define BOOST_NETWORK_HTTP_SERVER_HPP_

#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/network/protocol/http/request.hpp>
#include <boost/network/protocol/http/response.hpp>
#include <boost/network/protocol/http/connection.hpp>
#include <boost/network/traits/string.hpp>

namespace boost { namespace network { namespace http {

    template <class Tag, class Handler>
    struct basic_server {
        typedef typename string<Tag>::type string_type;
        typedef basic_request<Tag> request;
        typedef basic_response<Tag> response;

        basic_server(string_type const & address,
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
                boost::bind(&basic_server<Tag,Handler>::handle_accept,
                            this, boost::asio::placeholders::error));
        }

        void run() {
            service_.run();
        }

        void stop() {
            // TODO Graceful stop here?
            service_.stop();
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
                    boost::bind(&basic_server<Tag,Handler>::handle_accept,
                                this, boost::asio::placeholders::error));
            }
        }
    };

    template <class Handler>
    struct server : basic_server<tags::http_server, Handler> {
        typedef basic_server<tags::http_server, Handler> server_base;

        server(typename server_base::string_type const & address,
               typename server_base::string_type const & port,
               Handler & handler)
            : server_base(address, port, handler) {}
        
    };

} // namespace http

} // namespace network

} // namespace boost

#endif // BOOST_NETWORK_HTTP_SERVER_HPP_

