// Copyright 2009 (c) Tarro, Inc.
// Copyright 2009 (c) Dean Michael Berris <mikhailberis@gmail.com>
// Copyright 2010 (c) Glyn Matthews
// Copyright 2003-2008 (c) Chris Kholhoff
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_NETWORK_HTTP_SERVER_HPP_
#define BOOST_NETWORK_HTTP_SERVER_HPP_

#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/network/protocol/http/response.hpp>
#include <boost/network/protocol/http/request.hpp>
#include <boost/network/protocol/http/connection.hpp>
#include <boost/network/traits/string.hpp>
#include <boost/network/protocol/http/server/sync_server.hpp>
#include <boost/network/protocol/http/server/async_server.hpp>

namespace boost { namespace network { namespace http {

    template <class Tag, class Handler>
    struct server_base :
        mpl::if_<
            is_async<Tag>
            , async_server_base<Tag, Handler>
            , typename mpl::if_<
                is_sync<Tag>
                , sync_server_base<Tag, Handler>
                , unsupported_tag<Tag>
            >::type
        >
    {};

    template <class Tag, class Handler>
    struct basic_server : server_base<Tag, Handler>::type
    {};

    template <class Handler>
    struct server : server_base<tags::http_server, Handler>::type {
        typedef typename server_base<tags::http_server, Handler>::type
            server_base;

        server(typename server_base::string_type const & address,
               typename server_base::string_type const & port,
               Handler & handler)
            : server_base(address, port, handler) {}
        
    };

    template <class Handler>
    struct async_server : server_base<tags::http_async_server, Handler>::type
    {
        typedef typename server_base<tags::http_async_server, Handler>::type
            server_base;

        async_server(typename server_base::string_type const & address,
                     typename server_base::string_type const & port,
                     Handler & handler,
                     utils::thread_pool & thread_pool)
            : server_base(address, port, handler, thread_pool) {}
    };

} // namespace http

} // namespace network

} // namespace boost

#endif // BOOST_NETWORK_HTTP_SERVER_HPP_

