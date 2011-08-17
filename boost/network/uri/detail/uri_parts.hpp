// Copyright 2009, 2010, 2011 Dean Michael Berris, Jeroen Habraken, Glyn Matthews.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_NETWORK_URL_DETAIL_URL_PARTS_HPP_
# define BOOST_NETWORK_URL_DETAIL_URL_PARTS_HPP_


# include <boost/fusion/include/vector.hpp>
# include <boost/fusion/sequence/intrinsic/at_c.hpp>


namespace boost {
namespace network {
namespace uri {
namespace detail {
template <
    class String
    >
struct iterator_range
    : boost::fusion::vector<
      typename String::const_iterator
    , typename String::const_iterator
    >
{ };


template <
    class String
    >
struct uri_parts
    : boost::fusion::vector<
      iterator_range<String>         // scheme
    , boost::fusion::vector<
            iterator_range<String>   // user_info
          , iterator_range<String>   // host
          , iterator_range<String>   // port
          , iterator_range<String>   // path
          >
    , iterator_range<String>         // query
    , iterator_range<String>         // fragment
    >
{ };
} // namespace detail
} // namespace uri
} // namespace network
} // namespace boost


#endif // BOOST_NETWORK_URL_DETAIL_URL_PARTS_HPP_
