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

#ifndef HTTP_SERVER3_HEADER_HPP
#define HTTP_SERVER3_HEADER_HPP

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

} // namespace http

} // namespace network

} // namespace boost

BOOST_FUSION_ADAPT_TPL_STRUCT(
    (Tag),
    (boost::network::http::request_header)(Tag),
    (typename boost::network::string<Tag>::type, name)
    (typename boost::network::string<Tag>::type, value)
    )

#endif // HTTP_SERVER3_HEADER_HPP
