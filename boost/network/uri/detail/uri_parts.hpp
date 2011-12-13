// Copyright 2009, 2010, 2011 Dean Michael Berris, Jeroen Habraken, Glyn Matthews.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_NETWORK_URL_DETAIL_URL_PARTS_HPP_
# define BOOST_NETWORK_URL_DETAIL_URL_PARTS_HPP_


# include <boost/optional.hpp>


namespace boost {
namespace network {
namespace uri {
namespace detail {
template <
    class String
    >
struct hierarchical_part {
    boost::optional<String> user_info;
    boost::optional<String> host;
    boost::optional<String> port;
    boost::optional<String> path;
};

template <
    class String
    >
struct uri_parts {
    String scheme;
    hierarchical_part<String> hier_part;
    boost::optional<String> query;
    boost::optional<String> fragment;

    void clear() {
        scheme.clear();
        hier_part.user_info = boost::optional<String>();
        hier_part.host = boost::optional<String>();
        hier_part.port = boost::optional<String>();
        hier_part.path = boost::optional<String>();
        query = boost::optional<String>();
        fragment = boost::optional<String>();
    }
};
} // namespace detail
} // namespace uri
} // namespace network
} // namespace boost


#endif // BOOST_NETWORK_URL_DETAIL_URL_PARTS_HPP_
