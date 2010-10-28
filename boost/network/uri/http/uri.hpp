#ifndef BOOST_NETWORK_URL_HTTP_URL_HPP_
#define BOOST_NETWORK_URL_HTTP_URL_HPP_

// Copyright 2009 Dean Michael Berris, Jeroen Habraken.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/cstdint.hpp>
#include <boost/algorithm/string/predicate.hpp>

#include <boost/network/protocol/http/tags.hpp>
#include <boost/network/traits/string.hpp>
#include <boost/network/uri/basic_uri_fwd.hpp>
#include <boost/network/uri/http/detail/parse_specific.hpp>

namespace boost { namespace network { namespace uri {

template <>
struct basic_uri<http::tags::http_default_8bit_tcp_resolve> : uri_base<http::tags::http_default_8bit_tcp_resolve> {
    typedef uri_base<http::tags::http_default_8bit_tcp_resolve>::string_type string_type;
    using uri_base<http::tags::http_default_8bit_tcp_resolve>::operator=;
    using uri_base<http::tags::http_default_8bit_tcp_resolve>::swap;

    basic_uri() : uri_base<http::tags::http_default_8bit_tcp_resolve>() {}
    basic_uri(uri_base<http::tags::http_default_8bit_tcp_resolve>::string_type const & uri) : uri_base<http::tags::http_default_8bit_tcp_resolve>(uri) {}

    // boost::uint16_t port() const {
    string_type port() const {
        static const char http_port[] = "80";
        static const char https_port[] = "443";

        return parts_.port ? *(parts_.port) :
            (boost::iequals(parts_.scheme, string_type("https")) ?
             string_type(https_port, https_port + std::strlen(https_port)) :
             string_type(http_port, http_port + std::strlen(http_port)));
    }

    string_type path() const {
        return (parts_.path == "") ? string_type("/") : parts_.path;
    }
};

// inline
// boost::uint16_t
// port(basic_uri<http::tags::http_default_8bit_tcp_resolve> const & uri) {
//     return uri.port();
// }
} // namespace uri
} // namespace network
} // namespace boost

#endif

