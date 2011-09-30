//
// header.hpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2008 Christopher M. Kohlhoff (chris at kohlhoff dot com)
// Copyright (c) 2009,2010 Dean Michael Berris (mikhailberis@gmail.com)
// Copyright (c) 2009 Tarroo, Inc.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_HEADER_HPP_20101122
#define BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_HEADER_HPP_20101122

#include <string>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/assign/list_of.hpp>

namespace boost { namespace network { namespace http {

    struct request_header {
        std::string name, value;
    };

    inline void swap(request_header & l, request_header & r) {
        swap(l.name, r.name);
        swap(l.value, r.value);
    }

    struct response_header {
        std::string name, value;
    };

    inline void swap(response_header & l, response_header & r) {
        std::swap(l.name, r.name);
        std::swap(l.value, r.value);
    }

} // namespace http

} // namespace network

} // namespace boost

BOOST_FUSION_ADAPT_STRUCT(
    boost::network::http::request_header,
    (std::string, name)
    (std::string, value)
    )

BOOST_FUSION_ADAPT_STRUCT(
    boost::network::http::response_header,
    (std::string, name)
    (std::string, value)
    )

#endif // BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_HEADER_HPP_20101122
