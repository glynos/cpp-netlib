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

#include <boost/network/traits/string.hpp>
#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/assign/list_of.hpp>

namespace boost { namespace network { namespace http {

    template <class Tag>
    struct request_header
    {
        typedef Tag tag;
        typedef typename string<Tag>::type string_type;
        string_type name, value;
    };

    template <class Tag>
    inline void swap(request_header<Tag> & l, request_header<Tag> & r) {
        swap(l.name, r.name);
        swap(l.value, r.value);
    }

    template <class Tag>
    struct response_header {
        typedef Tag tag;
        typedef typename string<Tag>::type string_type;
        string_type name, value;
    };

    template <class Tag>
    void swap(response_header<Tag> & l, response_header<Tag> & r) {
        std::swap(l.name, r.name);
        std::swap(l.value, r.value);
    }

} // namespace http

} // namespace network

} // namespace boost

BOOST_FUSION_ADAPT_TPL_STRUCT(
    (Tag),
    (boost::network::http::request_header)(Tag),
    (typename boost::network::string<Tag>::type, name)
    (typename boost::network::string<Tag>::type, value)
    )

BOOST_FUSION_ADAPT_TPL_STRUCT(
    (Tag),
    (boost::network::http::response_header) (Tag),
    (typename boost::network::string<Tag>::type, name)
    (typename boost::network::string<Tag>::type, value)
    )

#endif // BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_HEADER_HPP_20101122
