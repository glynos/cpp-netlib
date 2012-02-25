// Copyright 2009, 2010, 2011 Dean Michael Berris, Jeroen Habraken, Glyn Matthews.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_NETWORK_URL_DETAIL_URL_PARTS_HPP_
# define BOOST_NETWORK_URL_DETAIL_URL_PARTS_HPP_


# include <boost/range/iterator_range.hpp>
# include <boost/optional.hpp>


namespace boost {
namespace network {
namespace uri {
namespace detail {
template <
    class FwdIter
    >
struct hierarchical_part {
    optional<iterator_range<FwdIter> > user_info;
    optional<iterator_range<FwdIter> > host;
    optional<iterator_range<FwdIter> > port;
    optional<iterator_range<FwdIter> > path;
};

template <
    class FwdIter
    >
struct uri_parts {
    iterator_range<FwdIter> scheme;
    hierarchical_part<FwdIter> hier_part;
    optional<iterator_range<FwdIter> > query;
    optional<iterator_range<FwdIter> > fragment;

    void clear() {
        scheme = iterator_range<FwdIter>();
        hier_part.user_info = optional<iterator_range<FwdIter> >();
        hier_part.host = optional<iterator_range<FwdIter> >();
        hier_part.port = optional<iterator_range<FwdIter> >();
        hier_part.path = optional<iterator_range<FwdIter> >();
        query = optional<iterator_range<FwdIter> >();
        fragment = optional<iterator_range<FwdIter> >();
    }
};
} // namespace detail
} // namespace uri
} // namespace network
} // namespace boost


#endif // BOOST_NETWORK_URL_DETAIL_URL_PARTS_HPP_
