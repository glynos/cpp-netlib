#ifndef BOOST_NETWORK_URL_DETAIL_PARSE_URL_HPP_
#define BOOST_NETWORK_URL_DETAIL_PARSE_URL_HPP_

// Copyright 2009 Dean Michael Berris, Jeroen Habraken.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/qi_attr.hpp>
#include <boost/spirit/include/qi_core.hpp>
#include <boost/spirit/include/qi_eps.hpp>
#include <boost/spirit/include/qi_grammar.hpp>
#include <boost/spirit/include/qi_omit.hpp>
#include <boost/spirit/include/qi_raw.hpp>
#include <boost/spirit/include/qi_rule.hpp>
#include <boost/spirit/include/qi_sequence.hpp>
#include <boost/spirit/include/version.hpp>

#include <boost/network/uri/detail/uri_parts.hpp>

namespace boost { namespace spirit { namespace traits {

    template <class Tag>
        struct transform_attribute<
            boost::network::uri::detail::uri_parts<Tag>,
            typename boost::network::uri::detail::uri_parts_tuple<Tag>::type
                > 
        {
            typedef typename boost::network::uri::detail::uri_parts_tuple<Tag>::type type;

            static type pre(boost::network::uri::detail::uri_parts<Tag> & parts) {
                typedef typename boost::network::string<Tag>::type string_type;

                boost::fusion::tuple<
                    boost::optional<string_type> &,
                    boost::optional<string_type> &,
                    boost::optional<boost::uint16_t> &,
                    string_type &
                        > hier_part =
                        boost::fusion::tie(
                                parts.user_info,
                                parts.host,
                                parts.port,
                                parts.path
                            );

                return boost::fusion::tie(
                        parts.scheme,
                        hier_part,
                        parts.query,
                        parts.fragment
                    );
            }

            static void post(boost::network::uri::detail::uri_parts<Tag> &, type const &) { }

#if SPIRIT_VERSION >= 0x2030
            static void fail(boost::network::uri::detail::uri_parts<Tag> & val) { }
#endif
        };

#if SPIRIT_VERSION < 0x2030
    template <typename Exposed, typename Transformed>
        struct transform_attribute<
            optional<Exposed>,
            Transformed,
            typename disable_if<is_same<optional<Exposed>, Transformed> >::type
                >
        {
            typedef Transformed & type;
        
            static Transformed & pre(optional<Exposed> & val) { 
                if (!val)
                    val = Transformed();
                return boost::get<Transformed>(val); 
            }
    
            static void post(optional<Exposed> &, Transformed const &) { }
        };
#endif

} // namespace traits

} // namespace spirit

} // namespace boost

namespace boost { namespace network { namespace uri {

    namespace detail {

        namespace qi = boost::spirit::qi;

        template <typename Iterator, class Tag>
            struct uri_grammar : qi::grammar<Iterator, uri_parts<Tag>()> {
                uri_grammar() : uri_grammar::base_type(start, "uri") {
                    // gen-delims = ":" / "/" / "?" / "#" / "[" / "]" / "@"
                    gen_delims = qi::char_(":/?#[]@");
                    // sub-delims = "!" / "$" / "&" / "'" / "(" / ")" / "*" / "+" / "," / ";" / "="
                    sub_delims = qi::char_("!$&'()*+,;=");
                    // reserved = gen-delims / sub-delims
                    reserved = gen_delims | sub_delims;
                    // unreserved = ALPHA / DIGIT / "-" / "." / "_" / "~"
                    unreserved = qi::alnum | qi::char_("-._~");
                    // pct-encoded = "%" HEXDIG HEXDIG
                    pct_encoded = qi::char_("%") >> qi::repeat(2)[qi::xdigit];

                    // pchar = unreserved / pct-encoded / sub-delims / ":" / "@"
                    pchar = qi::raw[
                        unreserved | pct_encoded | sub_delims | qi::char_(":@")
                    ];

                    // segment = *pchar
                    segment = qi::raw[*pchar];
                    // segment-nz = 1*pchar
                    segment_nz = qi::raw[+pchar];
                    // segment-nz-nc = 1*( unreserved / pct-encoded / sub-delims / "@" )
                    segment_nz_nc = qi::raw[
                        +(unreserved | pct_encoded | sub_delims | qi::char_("@"))
                    ];
                    // path-abempty  = *( "/" segment )
                    path_abempty = qi::raw[*(qi::char_("/") >> segment)];
                    // path-absolute = "/" [ segment-nz *( "/" segment ) ]
                    path_absolute = qi::raw[
                            qi::char_("/")
                        >>  -(segment_nz >> *(qi::char_("/") >> segment))
                    ];
                    // path-rootless = segment-nz *( "/" segment )
                    path_rootless = qi::raw[
                        segment_nz >> *(qi::char_("/") >> segment)
                    ];
                    // path-empty = 0<pchar>
                    path_empty = qi::eps;

                    // scheme = ALPHA *( ALPHA / DIGIT / "+" / "-" / "." )
                    scheme = qi::alpha >> *(qi::alnum | qi::char_("+.-"));
 
                    // user_info = *( unreserved / pct-encoded / sub-delims / ":" )
                    user_info = qi::raw[
                        *(unreserved | pct_encoded | sub_delims | qi::char_(":"))
                    ];

                    // dec-octet = DIGIT / %x31-39 DIGIT / "1" 2DIGIT / "2" %x30-34 DIGIT / "25" %x30-35
                    dec_octet =
                            !(qi::lit('0') >> qi::digit)
                        >>  qi::raw[
                                qi::uint_parser<boost::uint8_t, 10, 1, 3>()
                            ];
                    // IPv4address = dec-octet "." dec-octet "." dec-octet "." dec-octet
                    ipv4address = qi::raw[
                        dec_octet >> qi::repeat(3)[qi::lit('.') >> dec_octet]
                    ];                    
                    // reg-name = *( unreserved / pct-encoded / sub-delims )
                    reg_name = qi::raw[
                        *(unreserved | pct_encoded | sub_delims)
                    ];
                    // TODO, host = IP-literal / IPv4address / reg-name
                    host = ipv4address | reg_name;
 
                    // query = *( pchar / "/" / "?" )
                    query = qi::raw[*(pchar | qi::char_("/?"))];
                    // fragment = *( pchar / "/" / "?" )
                    fragment = qi::raw[*(pchar | qi::char_("/?"))];

                    // hier-part = "//" authority path-abempty / path-absolute / path-rootless / path-empty
                    // authority = [ userinfo "@" ] host [ ":" port ]
                    hier_part =
                            (
                                "//"
                            >>  -(user_info >> '@')
                            >>  host
                            >>  -(':' >> qi::ushort_)
                            >>  path_abempty
                            )
                        |
                            (
                                qi::attr(optional<typename boost::network::string<Tag>::type>())
                            >>  qi::attr(optional<typename boost::network::string<Tag>::type>())
                            >>  qi::attr(optional<boost::uint16_t>())
                            >>  (
                                    path_absolute
                                |   path_rootless
                                |   path_empty
                                )
                            );

                    start = uri;
                    uri = 
                            scheme >> ':'
                        >>  hier_part
                        >>  -('?' >> query)
                        >>  -('#' >> fragment);
                }

                typedef typename string<Tag>::type string_type;

                qi::rule<Iterator, typename string_type::value_type()>
                    gen_delims, sub_delims, reserved, unreserved;
                qi::rule<Iterator, string_type()>
                    pct_encoded, pchar;

                qi::rule<Iterator, string_type()>
                    segment, segment_nz, segment_nz_nc;
                qi::rule<Iterator, string_type()>
                    path_abempty, path_absolute, path_rootless, path_empty;

                qi::rule<Iterator, string_type()>
                    dec_octet, ipv4address, reg_name, host;

                qi::rule<Iterator, string_type()>
                    scheme, user_info, query, fragment;

                qi::rule<Iterator, boost::fusion::tuple<
                    optional<string_type> &,
                    optional<string_type> &,
                    optional<boost::uint16_t> &,
                    string_type &
                        >()> hier_part;

                // start rule of grammar
                qi::rule<Iterator, uri_parts<Tag>()> start;

                // actual uri parser
                qi::rule<Iterator, typename uri_parts_tuple<Tag>::type()> uri;
            };

        template <class Tag>
            inline bool parse_specific(uri_parts<Tag> & parts) {
                return true;
            }

        template <class Range, class Tag>
            inline bool parse_uri(Range & range, uri_parts<Tag> & parts) {
                typedef typename range_iterator<Range>::type iterator;

                iterator start_ = begin(range);
                iterator end_ = end(range);

                uri_grammar<iterator, Tag> grammar;

                bool ok = qi::parse(start_, end_, grammar, parts);

                if (ok)
                    ok = parse_specific(parts);

                return ok && start_ == end_;
            }

    } // namespace detail

} // namespace uri

} // namespace network

} // namespace boost

#endif

