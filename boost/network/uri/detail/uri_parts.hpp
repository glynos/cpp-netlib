#ifndef BOOST_NETWORK_URL_DETAIL_URL_PARTS_HPP_
#define BOOST_NETWORK_URL_DETAIL_URL_PARTS_HPP_

// Copyright 2009 Dean Michael Berris, Jeroen Habraken.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt of copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/cstdint.hpp>
#include <boost/fusion/tuple.hpp>
#include <boost/optional.hpp>

#include <boost/network/traits/string.hpp>

namespace boost { namespace network { namespace uri {

namespace detail {

template <class Tag>
struct uri_parts {
    typedef typename string<Tag>::type string_type;

    string_type scheme;
    optional<string_type> user_info;
    optional<string_type> host;
    optional<boost::uint16_t> port;
    string_type path;
    optional<string_type> query;
    optional<string_type> fragment;
};

template <class Tag>
struct uri_parts_tuple {
    typedef typename string<Tag>::type string_type;

    typedef typename boost::fusion::tuple<
        string_type &,
        boost::fusion::tuple<
            optional<string_type> &,
            optional<string_type> &,
            optional<boost::uint16_t> &,
            string_type &
            >,
                optional<string_type> &,
                optional<string_type> &
                > type;
};

template <class Tag>
inline void swap(uri_parts<Tag> & l, uri_parts<Tag> & r) {
    using std::swap;

    swap(l.scheme, r.scheme);
    swap(l.user_info, r.user_info);
    swap(l.host, r.host);
    swap(l.port, r.port);
    swap(l.path, r.path);
    swap(l.query, r.query);
    swap(l.fragment, r.fragment);
}

template <class Tag>
inline
bool operator==(uri_parts<Tag> const & l, uri_parts<Tag> const & r) {
    return (l.scheme == r.scheme) &&
        (l.user_info == r.user_info) &&
        (l.host == r.host) &&
        (l.port == r.port) &&
        (l.path == r.path) &&
        (l.query == r.query) &&
        (l.fragment == r.fragment);
}

template <class Tag>
inline
bool operator!=(uri_parts<Tag> const & l, uri_parts<Tag> const & r) {
    return !(l == r);
}
} // namespace detail
} // namespace uri
} // namespace network
} // namespace boost

#endif

