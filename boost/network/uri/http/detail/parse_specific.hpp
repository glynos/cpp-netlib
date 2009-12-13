#ifndef BOOST_NETWORK_URL_HTTP_DETAIL_PARSE_SPECIFIC_HPP_
#define BOOST_NETWORK_URL_HTTP_DETAIL_PARSE_SPECIFIC_HPP_

// Copyright 2009 Dean Michael Berris.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt of copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/uri/http/detail/uri_parts.hpp>
#include <boost/network/uri/detail/parse_uri.hpp>
#include <boost/network/uri/detail/constants.hpp>
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
            bool parse_specific<
                string<tags::http>::type,
                tags::http
                    >(
                            string<tags::http>::type & range,
                            uri_parts<tags::http> & parts
                     ) 
            {
                // Require that parts.scheme is either http or https
                if (parts.scheme.size() < 4)
                    return false;
                if (parts.scheme.substr(0, 4) != "http")
                    return false;
                if (parts.scheme.size() == 5) {
                    if (parts.scheme[4] != 's')
                        return false;
                } else if (parts.scheme.size() > 5)
                    return false;
                
                using spirit::qi::parse;
                using spirit::qi::lit;
                using spirit::ascii::char_;
                using spirit::ascii::space;
                using spirit::ascii::alnum;
                using spirit::ascii::punct;
                using spirit::qi::lexeme;
                using spirit::qi::uint_;
                using spirit::qi::digit;
                using spirit::qi::rule;
                
                typedef string<tags::http>::type string_type;
                typedef range_iterator<string_type>::type iterator;

                iterator start_ = begin(range);
                iterator end_ = end(range);
                fusion::tuple<
                    optional<string_type> &,
                    string_type &,
                    optional<uint32_t> &,
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

                hostname<tags::http>::parser<iterator> hostname;
                bool ok = parse(
                        start_, end_,
                        (
                         lit("//")
                         >> -lexeme[
                            *((alnum|punct) - '@')
                            >> '@'
                            ]
                         >> hostname
                         >> -lexeme[':' >> uint_]
                         >> -lexeme['/' >> *((alnum|punct) - '?')]
                         >> -lexeme['?' >> *((alnum|punct) - '#')]
                         >> -lexeme['#' >> *(alnum|punct)]
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

