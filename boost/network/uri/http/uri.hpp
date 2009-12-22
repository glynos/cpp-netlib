#ifndef BOOST_NETWORK_URL_HTTP_URL_HPP_
#define BOOST_NETWORK_URL_HTTP_URL_HPP_

// Copyright 2009 Dean Michael Berris, Jeroen Habraken.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/fusion/tuple.hpp>

#include <boost/network/tags.hpp>
#include <boost/network/uri/basic_uri_fwd.hpp>
#include <boost/network/traits/string.hpp>
#include <boost/network/uri/http/detail/parse_specific.hpp>
#include <boost/network/uri/http/detail/uri_parts.hpp>
#include <boost/network/uri/http/uri_concept.hpp>

namespace boost { namespace network { namespace uri {

    template <>
        struct basic_uri<tags::http> : uri_base<tags::http> {
            using uri_base<tags::http>::string_type;
            using uri_base<tags::http>::operator=;
            using uri_base<tags::http>::swap;

            basic_uri() : uri_base<tags::http>() {}
            basic_uri(uri_base<tags::http>::string_type const & uri) : uri_base<tags::http>(uri) {}

            string_type host() const {
                return parts_.host;
            }

            uint16_t port() const {
                return parts_.port ? *parts_.port : 
                    (parts_.scheme == "https" ? 443u : 80u);
            }

            string_type path() const {
                return string_type("/") + (parts_.path ? *parts_.path : string_type());
            }

            string_type query() const {
                return parts_.query ? *parts_.query : string_type();
            }

            string_type fragment() const {
                return parts_.fragment ? *parts_.fragment : string_type();
            }

            string_type user_info() const {
                return parts_.user_info ? *parts_.user_info : string_type();
            }

        };

    inline 
        basic_uri<tags::http>::string_type 
        host(basic_uri<tags::http> const & uri) {
            return uri.host();
        }

    inline
        uint16_t
        port(basic_uri<tags::http> const & uri) {
            return uri.port();
        }

    inline
        basic_uri<tags::http>::string_type
        path(basic_uri<tags::http> const & uri) {
            return uri.path();
        }

    inline 
        basic_uri<tags::http>::string_type
        query(basic_uri<tags::http> const & uri) {
            return uri.query();
        }

    inline 
        basic_uri<tags::http>::string_type
        fragment(basic_uri<tags::http> const & uri) {
            return uri.fragment();
        }

    inline
        basic_uri<tags::http>::string_type
        user_info(basic_uri<tags::http> const & uri) {
            return uri.user_info();
        }

    BOOST_CONCEPT_ASSERT((HttpURI<basic_uri<tags::http> >));

} // namespace uri

} // namespace network

} // namespace boost

#endif

