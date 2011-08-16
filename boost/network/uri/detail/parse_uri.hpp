// Copyright 2009, 2010, 2011 Dean Michael Berris, Jeroen Habraken, Glyn Matthews.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_NETWORK_URL_DETAIL_PARSE_URL_HPP_
# define BOOST_NETWORK_URL_DETAIL_PARSE_URL_HPP_


# ifdef BOOST_NETWORK_NO_LIB
#  define BOOST_NETWORK_INLINE inline
# else
#  define BOOST_NETWORK_INLINE
# endif // BOOST_NETWORK_NO_LIB


#include <boost/network/uri/detail/uri_parts.hpp>
#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/version.hpp>
#include <boost/spirit/repository/include/qi_iter_pos.hpp>


namespace boost {
namespace network {
namespace uri {
namespace detail {
namespace qi = boost::spirit::qi;

template <
    class String,
    typename Iterator
    >
struct uri_grammar : qi::grammar<Iterator, detail::uri_parts<String>()> {

    uri_grammar() : uri_grammar::base_type(start, "uri") {
        using boost::spirit::repository::qi::iter_pos;

        // gen-delims = ":" / "/" / "?" / "#" / "[" / "]" / "@"
        gen_delims %= qi::char_(":/?#[]@");
        // sub-delims = "!" / "$" / "&" / "'" / "(" / ")" / "*" / "+" / "," / ";" / "="
        sub_delims %= qi::char_("!$&'()*+,;=");
        // reserved = gen-delims / sub-delims
        reserved %= gen_delims | sub_delims;
        // unreserved = ALPHA / DIGIT / "-" / "." / "_" / "~"
        unreserved %= qi::alnum | qi::char_("-._~");
        // pct-encoded = "%" HEXDIG HEXDIG
        pct_encoded %= qi::char_("%") >> qi::repeat(2)[qi::xdigit];

        // pchar = unreserved / pct-encoded / sub-delims / ":" / "@"
        pchar %= qi::raw[
            unreserved | pct_encoded | sub_delims | qi::char_(":@")
            ];

        // segment = *pchar
        segment %= qi::raw[*pchar];
        // segment-nz = 1*pchar
        segment_nz %= qi::raw[+pchar];
        // segment-nz-nc = 1*( unreserved / pct-encoded / sub-delims / "@" )
        segment_nz_nc %= qi::raw[
            +(unreserved | pct_encoded | sub_delims | qi::char_("@"))
            ];
        // path-abempty  = *( "/" segment )
        path_abempty %=
               iter_pos
            >> qi::omit[qi::raw[*(qi::char_("/") >> segment)]]
            >> iter_pos
            ;
        // path-absolute = "/" [ segment-nz *( "/" segment ) ]
        path_absolute %=
               iter_pos
            >> qi::omit[qi::raw[
                                qi::char_("/")
                                >>  -(segment_nz >> *(qi::char_("/") >> segment))
                                ]]
            >> iter_pos
            ;
        // path-rootless = segment-nz *( "/" segment )
        path_rootless %=
               iter_pos
            >> qi::omit[qi::raw[
                                segment_nz >> *(qi::char_("/") >> segment)
                                ]]
            >> iter_pos
            ;
        // path-empty = 0<pchar>
        path_empty %=
               iter_pos
            >> qi::omit[qi::eps]
            >> iter_pos
            ;

        // scheme = ALPHA *( ALPHA / DIGIT / "+" / "-" / "." )
        scheme %=
               iter_pos
            >> qi::omit[qi::alpha >> *(qi::alnum | qi::char_("+.-"))]
            >> iter_pos
            ;

        // user_info = *( unreserved / pct-encoded / sub-delims / ":" )
        user_info %=
               iter_pos
            >> qi::omit[qi::raw[*(unreserved | pct_encoded | sub_delims | qi::char_(":"))]]
            >> iter_pos
            ;

        // dec-octet = DIGIT / %x31-39 DIGIT / "1" 2DIGIT / "2" %x30-34 DIGIT / "25" %x30-35
        dec_octet %=
            !(qi::lit('0') >> qi::digit)
            >>  qi::raw[
                qi::uint_parser<boost::uint8_t, 10, 1, 3>()
                ];

        // IPv4address = dec-octet "." dec-octet "." dec-octet "." dec-octet
        ipv4address %= qi::raw[
            dec_octet >> qi::repeat(3)[qi::lit('.') >> dec_octet]
            ];

        // reg-name = *( unreserved / pct-encoded / sub-delims )
        reg_name %= qi::raw[
            *(unreserved | pct_encoded | sub_delims)
            ];

        // TODO, host = IP-literal / IPv4address / reg-name
        host %=
               iter_pos
            >> qi::omit[ipv4address | reg_name]
            >> iter_pos
            ;

        // port %= qi::ushort_;
        port %=
               iter_pos
            >> qi::omit[*qi::digit]
            >> iter_pos
            ;

        // query = *( pchar / "/" / "?" )
        query %=
               iter_pos
            >> qi::omit[qi::raw[*(pchar | qi::char_("/?"))]]
            >> iter_pos
            ;

        // fragment = *( pchar / "/" / "?" )
        fragment %=
               iter_pos
            >> qi::omit[qi::raw[*(pchar | qi::char_("/?"))]]
            >> iter_pos
            ;

        // hier-part = "//" authority path-abempty / path-absolute / path-rootless / path-empty
        // authority = [ userinfo "@" ] host [ ":" port ]
        hier_part %=
            (
                "//"
                >>  -(user_info >> '@')
                >>  host
                >> -(':' >> port)
                >>  path_abempty
                )
            |
            (
                    qi::attr(iterator_range<String>())
                >>  qi::attr(iterator_range<String>())
                >>  qi::attr(iterator_range<String>())
                >>  (
                    path_absolute
                    |   path_rootless
                    |   path_empty
                    )
                )
            ;

        start %=
            scheme >> ':'
            >> hier_part
            >>  -('?' >> query)
            >>  -('#' >> fragment)
            ;
    }

    qi::rule<Iterator, typename String::value_type()>
    gen_delims, sub_delims, reserved, unreserved;
    qi::rule<Iterator, String()>
    pct_encoded, pchar;

    qi::rule<Iterator, String()>
    segment, segment_nz, segment_nz_nc;
    qi::rule<Iterator, iterator_range<String>()>
    path_abempty, path_absolute, path_rootless, path_empty;

    qi::rule<Iterator, String()>
    dec_octet, ipv4address, reg_name;

    qi::rule<Iterator, iterator_range<String>()>
    host, port;

    qi::rule<Iterator, iterator_range<String>()>
    scheme, user_info, query, fragment;

    qi::rule<Iterator, boost::fusion::vector<iterator_range<String>,
                                             iterator_range<String>,
                                             iterator_range<String>,
                                             iterator_range<String> >()>
    hier_part;

    // actual uri parser
    qi::rule<Iterator, uri_parts<String>()> start;

};


BOOST_NETWORK_INLINE
bool parse(std::string::const_iterator first,
           std::string::const_iterator last,
           uri_parts<std::string> &parts) {
    namespace qi = boost::spirit::qi;
    static detail::uri_grammar<std::string, std::string::const_iterator> grammar;
    bool is_valid = qi::parse(first, last, grammar, parts);
    return is_valid && (first == last);
}

BOOST_NETWORK_INLINE
bool parse(std::wstring::const_iterator first,
           std::wstring::const_iterator last,
           uri_parts<std::wstring> &parts) {
    namespace qi = boost::spirit::qi;
    static uri_grammar<std::wstring, std::wstring::const_iterator> grammar;
    bool is_valid = qi::parse(first, last, grammar, parts);
    return is_valid && (first == last);
}
} // namespace detail
} // namespace uri
} // namespace network
} // namespace boost


#endif // BOOST_NETWORK_URL_DETAIL_PARSE_URL_HPP_

