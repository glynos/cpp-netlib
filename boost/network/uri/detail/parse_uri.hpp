#ifndef BOOST_NETWORK_URL_DETAIL_PARSE_URL_HPP_
#define BOOST_NETWORK_URL_DETAIL_PARSE_URL_HPP_

// Copyright 2009 Dean Michael Berris, Jeroen Habraken.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/fusion/tuple.hpp>
#include <boost/network/uri/detail/uri_parts.hpp>

#include <boost/spirit/include/qi_core.hpp>
#include <boost/spirit/include/qi_sequence.hpp>
#include <boost/spirit/include/qi_parse.hpp>
#include <boost/spirit/include/qi_char_.hpp>
#include <boost/spirit/include/qi_lexeme.hpp>

namespace boost { namespace network { namespace uri {

    namespace detail {

        template <class Range, class Tag>
            inline bool parse_specific(Range & range, uri_parts<Tag> & parts) {
                return true;
            }

        template <class Range, class Tag>
            inline bool parse_uri(Range & range, uri_parts<Tag> & parts) {
                namespace qi = boost::spirit::qi;                

                typedef typename range_iterator<Range>::type iterator;
                typedef typename string<Tag>::type string_type;

                iterator start_ = begin(range);
                iterator end_ = end(range);
                fusion::tuple<string_type&,string_type&> result =
                    fusion::tie(parts.scheme,parts.scheme_specific_part);

                bool ok = qi::parse(
                        start_, end_, 
                        (
                            qi::lexeme[(qi::alpha >> *(qi::alnum | qi::char_("+.-")))]
                         >> ':'
                         >> 
                            +(qi::char_ - (qi::cntrl | qi::space))
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

