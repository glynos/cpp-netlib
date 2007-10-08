
//          Copyright Dean Michael Berris 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef __NETWORK_PROTOCOL_HTTP_REQUEST_20070908_1_HPP__
#define __NETWORK_PROTOCOL_HTTP_REQUEST_20070908_1_HPP__

// Implement the HTTP Request Object

#include <boost/network/message.hpp>

// forward declarations
namespace boost { namespace network { namespace http {

    template <class tag>
    class basic_request;

    typedef basic_request<tags::default_> request;

}; // namespace http

}; // namespace network

}; // namespace boost

#include <boost/network/protocol/http/client.hpp>
#include <boost/network/protocol/http/impl/request.hpp>

#endif // __NETWORK_PROTOCOL_HTTP_REQUEST_20070908-1_HPP__
