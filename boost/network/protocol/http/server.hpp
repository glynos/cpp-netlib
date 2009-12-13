// Copyright 2009 (c) Tarro, Inc.
// Copyright 2009 (c) Dean Michael Berris <mikhailberis@gmail.com>
// Copyright 2003-2008 (c) Chris Kholhoff
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_NETWORK_HTTP_SERVER_HPP_
#define BOOST_NETWORK_HTTP_SERVER_HPP_

#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <string>
#include <boost/asio.hpp>
#include <boost/network/protocol/http/connection.hpp>

namespace boost { namespace network { namespace http {

    using boost::shared_ptr;
    using std::string;
    using boost::asio::ip::tcp;
    using boost::bind;

    template <class Handler>
    struct server {
        server(string const & address, string const & port, Handler & handler)
        : handler_(handler)
        , service_()
        , acceptor_(service_)
        , new_connection(new connection<Handler>(service_, handler))
        {
            tcp::resolver resolver(service_);
            tcp::resolver::query query(address, port);
            tcp::endpoint endpoint = *resolver.resolve(query);
            acceptor_.open(endpoint.protocol());
            acceptor_.set_option(tcp::acceptor::reuse_address(true));
            acceptor_.bind(endpoint);
            acceptor_.listen();
            acceptor_.async_accept(new_connection->socket(),
                bind(&server<Handler>::handle_accept, this, boost::asio::placeholders::error));
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
        io_service service_;
        tcp::acceptor acceptor_;
        shared_ptr<connection<Handler> > new_connection;

        void handle_accept(boost::system::error_code const & ec) {
            if (!ec) {
                new_connection->start();
                new_connection.reset(new connection<Handler>(service_, handler_));
                acceptor_.async_accept(new_connection->socket(),
                    bind(&server<Handler>::handle_accept, this, boost::asio::placeholders::error));
            }
        }
    };

} // namespace http

} // namespace network

} // namespace boost

#endif // BOOST_NETWORK_HTTP_SERVER_HPP_

