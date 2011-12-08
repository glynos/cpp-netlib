// Copyright 2009, 2010, 2011 Dean Michael Berris, Jeroen Habraken, Glyn Matthews.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_NETWORK_URL_DETAIL_URL_PARTS_HPP_
# define BOOST_NETWORK_URL_DETAIL_URL_PARTS_HPP_


# include <boost/range/iterator_range.hpp>


namespace boost {
namespace network {
namespace uri {
namespace detail {
template <
    class FwdIter
    >
struct hierarchical_part {
    iterator_range<FwdIter> user_info, host, port, path;

    hierarchical_part()
    {}

    hierarchical_part(FwdIter begin, FwdIter end)
        : user_info(begin, end)
        , host(begin, end)
        , port(begin, end)
        , path(begin, end)
    { }
};

template <
    class FwdIter
    >
struct uri_parts {
    iterator_range<FwdIter> scheme;
    hierarchical_part<FwdIter> hier_part;
    iterator_range<FwdIter> query;
    iterator_range<FwdIter> fragment;

    uri_parts()
    {}

    uri_parts(FwdIter begin, FwdIter end)
        : scheme(begin, end)
        , hier_part(begin, end)
        , query(begin, end)
        , fragment(begin, end)
    { }
};
} // namespace detail
} // namespace uri
} // namespace network
} // namespace boost


#endif // BOOST_NETWORK_URL_DETAIL_URL_PARTS_HPP_
