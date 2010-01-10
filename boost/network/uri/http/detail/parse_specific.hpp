#ifndef BOOST_NETWORK_URL_HTTP_DETAIL_PARSE_SPECIFIC_HPP_
#define BOOST_NETWORK_URL_HTTP_DETAIL_PARSE_SPECIFIC_HPP_

// Copyright 2009 Dean Michael Berris, Jeroen Habraken.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt of copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/algorithm/string/predicate.hpp>
#include <boost/network/uri/http/detail/uri_parts.hpp>
#include <boost/network/uri/detail/parse_uri.hpp>
#include <boost/network/traits/string.hpp>

namespace boost { namespace network { namespace uri { 

    namespace detail {

        template <class Tag>
        struct hostname {

            typedef typename string<Tag>::type string_type;

            template <class Iterator>
            struct parser : spirit::qi::grammar<Iterator, string_type()> {
                typedef spirit::qi::grammar<Iterator, string_type()> base_type;
                parser() : parser::base_type(start, "hostname") {
                    using spirit::qi::eps;
                    using spirit::qi::alnum;
                    using spirit::qi::_val;
                    using spirit::qi::_1;
                    using spirit::qi::lexeme;
                    using spirit::ascii::char_;
                    start = eps [_val = ""]
                        >> +(
                            alnum[ _val += _1 ]
                            | lexeme[char_('.') [ _val += '.' ] >> alnum [ _val += _1] ]
                            | lexeme[char_('-') [ _val += '-' ] >> alnum [ _val += _1] ]
                            );
                }

                spirit::qi::rule<Iterator, string_type()> start;
            };
        };

        template <>
            inline bool parse_specific<
                string<tags::http_default_8bit_tcp_resolve>::type,
                tags::http_default_8bit_tcp_resolve
                    >(
                            string<tags::http_default_8bit_tcp_resolve>::type & range,
                            uri_parts<tags::http_default_8bit_tcp_resolve> & parts
                     ) 
            {
                namespace qi = spirit::qi;

                // Require that parts.scheme is either http or https, case insensitive
                if (parts.scheme.size() < 4 or parts.scheme.size() > 5)
                    return false;
                if (parts.scheme.size() == 4) {
                    if (not boost::iequals(parts.scheme.substr(0, 4), "http"))
                        return false;
                } else {  // size is 5
                    if (not boost::iequals(parts.scheme.substr(0, 5), "https"))
                        return false;
                }
                
                typedef string<tags::http_default_8bit_tcp_resolve>::type string_type;
                typedef range_iterator<string_type>::type iterator;

                iterator start_ = begin(range);
                iterator end_ = end(range);
                fusion::tuple<
                    optional<string_type> &,
                    string_type &,
                    optional<uint16_t> &,
                    optional<string_type> &,
                    optional<string_type> &,
                    optional<string_type> &
                        > result =
                        fusion::tie(
                                parts.user_info,
                                parts.host,
                                parts.port,
                                parts.path,
                                parts.query,
                                parts.fragment
                           );

                qi::rule<iterator, string_type::value_type()> gen_delims = qi::char_(":/?#[]@");
                qi::rule<iterator, string_type::value_type()> sub_delims = qi::char_("!$&'()*+,;=");
                qi::rule<iterator, string_type::value_type()> reserved = gen_delims | sub_delims;
                qi::rule<iterator, string_type::value_type()> unreserved = qi::alnum | qi::char_("-._~");
                qi::rule<iterator, string_type()> pct_encoded = qi::raw[qi::char_("%") > qi::repeat(2)[qi::xdigit]];
                qi::rule<iterator, string_type()> pchar = qi::raw[unreserved | pct_encoded | sub_delims | qi::char_(":@")];
                
                hostname<tags::http_default_8bit_tcp_resolve>::parser<iterator> hostname;
                bool ok = parse(
                        start_, end_,
                        (
                         qi::lit("//")
                         >> -qi::lexeme[qi::raw[*(unreserved | pct_encoded | sub_delims | qi::char_(":"))] >> '@']
                         >> hostname
                         >> -qi::lexeme[':' >> qi::ushort_]
                         >> -qi::lexeme['/' > qi::raw[*pchar >> *('/' > *pchar)]]
                         >> -qi::lexeme['?' >> qi::raw[*(pchar | qi::char_("/?"))]]
                         >> -qi::lexeme['#' >> qi::raw[*(pchar | qi::char_("/?"))]]
                        ),
                        result
                        );

                return ok && start_ == end_;
            }

    } // namespace detail

} // namespace uri

} // namespace network

} // namespace boost

#endif

