#ifndef BOOST_NETWORK_URL_DETAIL_PARSE_URL_HPP_
#define BOOST_NETWORK_URL_DETAIL_PARSE_URL_HPP_

// Copyright 2009 Dean Michael Berris.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/fusion/tuple.hpp>
#include <boost/network/uri/detail/uri_parts.hpp>

namespace boost { namespace network { namespace uri {

    namespace detail {

        template <class Range, class Tag>
            inline bool parse_specific(Range & range, uri_parts<Tag> & parts) {
                return true;
            }

        template <class Range, class Tag>
            inline bool parse_uri(Range & range, uri_parts<Tag> & parts) {
                using spirit::qi::parse;
                using spirit::qi::lexeme;
                using spirit::ascii::char_;
                using spirit::ascii::cntrl;
                using spirit::ascii::alnum;
                using spirit::ascii::space;
                using namespace spirit::qi::labels;
                using fusion::tie;

                typedef typename range_iterator<Range>::type iterator;
                typedef typename string<Tag>::type string_type;

                iterator start_ = begin(range);
                iterator end_ = end(range);
                fusion::tuple<string_type&,string_type&> result =
                    tie(parts.scheme,parts.scheme_specific_part);

                bool ok = parse(
                        start_, end_, 
                        (
                            +((alnum|char_("+.-")) - ':')
                         >> ':'
                         >> 
                            +(char_ - (cntrl|space))
                        ),
                        result
                        );

                if (ok) {
                    ok = parse_specific(
                            parts.scheme_specific_part,
                            parts
                            );
                }

                return ok && start_ == end_;
            }

    } // namespace detail

} // namespace uri

} // namespace network

} // namespace boost

#endif

