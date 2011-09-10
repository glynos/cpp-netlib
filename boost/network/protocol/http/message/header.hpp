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

    template <class String>
    struct unsupported_string;

    struct request_header_narrow {
        typedef std::string string_type;
        std::string name, value;
    };

    struct request_header_wide {
        typedef std::wstring string_type;
        std::wstring name, value;
    };

    template <class String, class Enable=void>
    struct request_header {
      typedef unsupported_string<String> type;
    };

    template <class String>
    struct request_header<String, typename enable_if<is_same<String, std::string> >::type> {
      typedef request_header_narrow type;
    };

    template <class String>
    struct request_header<String, typename enable_if<is_same<String, std::wstring> >::type> {
      typedef request_header_wide type;
    };

    inline void swap(request_header_narrow & l, request_header_narrow & r) {
        swap(l.name, r.name);
        swap(l.value, r.value);
    }

    inline void swap(request_header_wide & l, request_header_wide & r) {
        swap(l.name, r.name);
        swap(l.value, r.value);
    }

    struct response_header_narrow {
        typedef std::string string_type;
        std::string name, value;
    };

    struct response_header_wide {
        typedef std::wstring string_type;
        std::wstring name, value;
    };

    template <class String, class Enable=void>
    struct response_header {
      typedef unsupported_string<String> type;
    };

    template <class String>
    struct response_header<String, typename enable_if<is_same<String, std::wstring> >::type> {
      typedef response_header_wide type;
    };

    template <class String>
    struct response_header<String, typename enable_if<is_same<String, std::string> >::type> {
      typedef response_header_narrow type;
    };

    inline void swap(response_header_narrow & l, response_header_narrow & r) {
        std::swap(l.name, r.name);
        std::swap(l.value, r.value);
    }

    inline void swap(response_header_wide & l, response_header_wide & r) {
        std::swap(l.name, r.name);
        std::swap(l.value, r.value);
    }
    
} // namespace http

} // namespace network

} // namespace boost

BOOST_FUSION_ADAPT_STRUCT(
    boost::network::http::request_header_narrow,
    (std::string, name)
    (std::string, value)
    )

BOOST_FUSION_ADAPT_STRUCT(
    boost::network::http::request_header_wide,
    (std::wstring, name)
    (std::wstring, value)
    )

BOOST_FUSION_ADAPT_STRUCT(
    boost::network::http::response_header_narrow,
    (std::string, name)
    (std::string, value)
    )

BOOST_FUSION_ADAPT_STRUCT(
    boost::network::http::response_header_wide,
    (std::wstring, name)
    (std::wstring, value)
    )

#endif // BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_HEADER_HPP_20101122
