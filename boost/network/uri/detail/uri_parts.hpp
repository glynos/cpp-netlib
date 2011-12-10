// Copyright 2009, 2010, 2011 Dean Michael Berris, Jeroen Habraken, Glyn Matthews.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_NETWORK_URL_DETAIL_URL_PARTS_HPP_
# define BOOST_NETWORK_URL_DETAIL_URL_PARTS_HPP_


# include <string>


namespace boost {
namespace network {
namespace uri {
namespace detail {
struct hierarchical_part {
    std::string user_info, host, port, path;

    void clear() {
        user_info.clear();
        host.clear();
        port.clear();
        path.clear();
    }
};

struct uri_parts {
    std::string scheme;
    hierarchical_part hier_part;
    std::string query;
    std::string fragment;

    void clear() {
        scheme.clear();
        hier_part.clear();
        query.clear();
        fragment.clear();
    }
};
} // namespace detail
} // namespace uri
} // namespace network
} // namespace boost


#endif // BOOST_NETWORK_URL_DETAIL_URL_PARTS_HPP_
