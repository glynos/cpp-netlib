#ifndef BOOST_NETWORK_URL_HTTP_DETAIL_URL_PARTS_HPP_
#define BOOST_NETWORK_URL_HTTP_DETAIL_URL_PARTS_HPP_

// Copyright 2009 Dean Michael Berris.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt of copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/uri/detail/uri_parts.hpp>
#include <boost/network/protocol/http/traits/string.hpp>

namespace boost { namespace network { namespace uri { 

    namespace detail {

        template <> struct uri_parts<tags::http> {
            typedef string<tags::http>::type string_type;
            string_type scheme;
            string_type scheme_specific_part;
            optional<string_type> user_info;
            string_type host;
            optional<uint32_t> port;
            optional<string_type> path;
            optional<string_type> query;
            optional<string_type> fragment;
        };

        template <>
            inline void swap<tags::http>(uri_parts<tags::http> & l, uri_parts<tags::http> & r) {
                using std::swap;
                swap(l.scheme, r.scheme);
                swap(l.scheme_specific_part, r.scheme_specific_part);
                swap(l.user_info, r.user_info);
                swap(l.host, r.host);
                swap(l.port, r.port);
                swap(l.path, r.path);
                swap(l.query, r.query);
                swap(l.fragment, r.fragment);
            }
        
    } // namespace detail

} // namespace uri

} // namespace network

} // namespace boost

#endif

