// Copyright 2010 Dean Michael Berris.
// Copyright 2012 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef NETWORK_PROTOCOL_HTTP_SERVER_STORAGE_BASE_HPP_20101210
#define NETWORK_PROTOCOL_HTTP_SERVER_STORAGE_BASE_HPP_20101210

#include <network/protocol/http/server/parameters.hpp>

namespace network { namespace http {

    struct server_storage_base {
        struct no_io_service {};
        struct has_io_service {};
    protected:
        template <class ArgPack>
        server_storage_base(ArgPack const & /* args */, no_io_service)
        : self_service_(new asio::io_service())
        , service_(*self_service_)
        {}

        template <class ArgPack>
        server_storage_base(ArgPack const & args, has_io_service)
        : self_service_(0)
        , service_(args[_io_service])
        {}

        ~server_storage_base() {
            delete self_service_;
            self_service_ = 0;
        }

        asio::io_service * self_service_;
        asio::io_service & service_;
    };
    
    
}  // namespace http
    
}  // namespace network
    
#endif /* NETWORK_PROTOCOL_HTTP_SERVER_STORAGE_BASE_HPP_20101210 */
