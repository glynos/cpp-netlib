#ifndef BOOST_NETWORK_URL_DETAIL_PARSE_URL_HPP_
#define BOOST_NETWORK_URL_DETAIL_PARSE_URL_HPP_

// Copyright 2009 Dean Michael Berris, Jeroen Habraken.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/fusion/tuple.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/qi_char_.hpp>
#include <boost/spirit/include/qi_core.hpp>
#include <boost/spirit/include/qi_eps.hpp>
#include <boost/spirit/include/qi_omit.hpp>
#include <boost/spirit/include/qi_parse.hpp>
#include <boost/spirit/include/qi_raw.hpp>
#include <boost/spirit/include/qi_rule.hpp>
#include <boost/spirit/include/qi_sequence.hpp>

#include <boost/network/uri/detail/uri_parts.hpp>

namespace boost { namespace network { namespace uri {

    namespace detail {

        template <class Tag>
            inline bool parse_specific(uri_parts<Tag> & parts) {
                return true;
            }

        template <class Range, class Tag>
            inline bool parse_uri(Range & range, uri_parts<Tag> & parts) {
                namespace qi = boost::spirit::qi;                

                typedef typename range_iterator<Range>::type iterator;

                typedef typename string<Tag>::type string_type;               
                typedef typename string<Tag>::type::value_type char_type;

                iterator start_ = begin(range);
                iterator end_ = end(range);
                fusion::tuple<
                    string_type &,
                    string_type &,
                    boost::optional<string_type> &,
                    boost::optional<string_type> &
                        > result =
                        fusion::tie(
                                parts.scheme,
                                parts.path,
                                parts.query,
                                parts.fragment
                            );

                // gen-delims = ":" / "/" / "?" / "#" / "[" / "]" / "@"
                qi::rule<iterator, char_type()> gen_delims, sub_delims;
                gen_delims = qi::char_(":/?#[]@");
                // sub-delims = "!" / "$" / "&" / "'" / "(" / ")" / "*" / "+" / "," / ";" / "="
                sub_delims = qi::char_("!$&'()*+,;=");
                // reserved = gen-delims / sub-delims
                qi::rule<iterator, char_type()> reserved;
                reserved = gen_delims | sub_delims;
                // unreserved = ALPHA / DIGIT / "-" / "." / "_" / "~"
                qi::rule<iterator, char_type()> unreserved;
                unreserved = qi::alnum | qi::char_("-._~");
                // pct-encoded = "%" HEXDIG HEXDIG
                qi::rule<iterator, string_type()> pct_encoded;
                pct_encoded = qi::char_("%") >> qi::repeat(2)[qi::xdigit];

                // pchar = unreserved / pct-encoded / sub-delims / ":" / "@"
                qi::rule<iterator, string_type()> pchar;
                pchar = qi::raw[
                    unreserved | pct_encoded | sub_delims | qi::char_(":@")
                ];

                // segment = *pchar
                qi::rule<iterator, string_type()> segment = qi::raw[*pchar];
                // segment-nz = 1*pchar
                qi::rule<iterator, string_type()> segment_nz = qi::raw[+pchar];
                // segment-nz-nc = 1*( unreserved / pct-encoded / sub-delims / "@" )
                qi::rule<iterator, string_type()> segment_nz_nc;
                segment_nz_nc = qi::raw[
                    +(unreserved | pct_encoded | sub_delims | qi::char_("@"))
                ];
                // path-abempty  = *( "/" segment )
                qi::rule<iterator, string_type()> path_abempty;
                path_abempty = qi::raw[*(qi::char_("/") >> segment)];
                // path-absolute = "/" [ segment-nz *( "/" segment ) ]
                qi::rule<iterator, string_type()> path_absolute;
                path_absolute = qi::raw[
                        qi::char_("/")
                    >>  -(segment_nz >> *(qi::char_("/") >> segment))
                ];
                // path-rootless = segment-nz *( "/" segment )
                qi::rule<iterator, string_type()> path_rootless;
                path_rootless = qi::raw[
                    segment_nz >> *(qi::char_("/") >> segment)
                ];
                // path-empty = 0<pchar>
                qi::rule<iterator, string_type()> path_empty = qi::eps;

                // scheme = ALPHA *( ALPHA / DIGIT / "+" / "-" / "." )
                qi::rule<iterator, string_type()> scheme;
                scheme = qi::alpha >> *(qi::alnum | qi::char_("+.-"));

                // user_info = *( unreserved / pct-encoded / sub-delims / ":" )
                qi::rule<iterator, string_type()> user_info;
                user_info = qi::raw[
                    *(unreserved | pct_encoded | sub_delims | qi::char_(":"))
                ];

                // reg-name = *( unreserved / pct-encoded / sub-delims )
                qi::rule<iterator, string_type()> reg_name;
                reg_name = qi::raw[*(unreserved | pct_encoded | sub_delims)];
                // FIXME, host = IP-literal / IPv4address / reg-name
                qi::rule<iterator, string_type()> host = reg_name;

                qi::rule<iterator, string_type()> query, fragment;
                // query = *( pchar / "/" / "?" )
                query = qi::raw[*(pchar | qi::char_("/?"))];
                // fragment = *( pchar / "/" / "?" )
                fragment = qi::raw[*(pchar | qi::char_("/?"))];

                bool ok = qi::parse(
                        start_, end_,  
                        (
                            scheme >> ':'
                        >>  (
                                  "//"
                              >>  qi::omit[
                                       -(user_info >> '@')    [phoenix::ref(parts.user_info) = qi::_1]
                                    >>  host                  [phoenix::ref(parts.host) = qi::_1]
                                    >> -(':' >> qi::ushort_)  [phoenix::ref(parts.port) = qi::_1]
                                  ]
                              >>  path_abempty
                            |   path_absolute
                            |   path_rootless
                            |   path_empty
                            )
                        >> -('?' >> query)
                        >> -('#' >> fragment)
                        ),
                        result
                    );

                if (ok) {
                    ok = parse_specific(
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

