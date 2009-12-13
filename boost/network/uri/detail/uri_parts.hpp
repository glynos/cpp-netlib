#ifndef BOOST_NETWORK_URL_DETAIL_URL_PARTS_HPP_
#define BOOST_NETWORK_URL_DETAIL_URL_PARTS_HPP_

// Copyright 2009 Dean Michael Berris.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt of copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/traits/string.hpp>

namespace boost { namespace network { namespace uri {

    namespace detail {

        template <class Tag>
            struct uri_parts {
                typedef typename string<Tag>::type string_type;
                string_type scheme;
                string_type scheme_specific_part;
            };

        template <class Tag>
            inline void swap(uri_parts<Tag> & l, uri_parts<Tag> & r) {
                using std::swap;
                swap(l.scheme, r.scheme);
                swap(l.scheme_specific_part, r.scheme_specific_part);
            }

        template <class Tag>
            inline 
            bool operator==(uri_parts<Tag> const & l, uri_parts<Tag> const & r) {
                return (l.scheme == r.scheme) && (l.scheme_specific_part == r.scheme_specific_part);
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

