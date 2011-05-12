// Copyright 2009, 2010, 2011 Dean Michael Berris, Jeroen Habraken, Glyn Matthews.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#ifndef __BOOST_NETWORK_URI_INC__
# define __BOOST_NETWORK_URI_INC__


# include <boost/network/traits/string.hpp>
# include <boost/network/tags.hpp>
# include <boost/range/iterator_range.hpp>
# include <boost/operators.hpp>
# include <boost/fusion/include/vector.hpp>
# include <boost/fusion/sequence/intrinsic/at_c.hpp>
# include <algorithm>


namespace boost {
namespace network {
namespace uri {
namespace details {
template <
    class Tag
    >
struct iterator_range
    : boost::fusion::vector<
      typename string<Tag>::type::const_iterator
    , typename string<Tag>::type::const_iterator
    >
{ };


template <
    class Tag
    >
struct uri_parts
    : boost::fusion::vector<
      iterator_range<Tag>
    , iterator_range<Tag>
    , iterator_range<Tag>
    , iterator_range<Tag>
    , iterator_range<Tag>
    , iterator_range<Tag>
    , iterator_range<Tag>
    >
{ };
} // namespace details

template <
    class Tag
    >
class basic_uri
    : public boost::equality_comparable<basic_uri<Tag> > {

public:

    typedef typename string<Tag>::type string_type;
    typedef typename string_type::iterator iterator_type;
    typedef boost::iterator_range<iterator_type> range_type;
    typedef typename string_type::const_iterator const_iterator_type;
    typedef boost::iterator_range<const_iterator_type> const_range_type;

    basic_uri() : is_valid_(false) {

    }

    explicit basic_uri(const string_type &uri)
        : uri_(uri), is_valid_(false) {
        parse();
    }

    basic_uri(const basic_uri &other)
        : uri_(other.uri_),
          uri_parts_(other.uri_parts_),
          is_valid_(other.is_valid_) {

    }

    basic_uri &operator = (const basic_uri &other) {
        basic_uri(other).swap(*this);
        return *this;
    }

    ~basic_uri() {

    }

    void swap(basic_uri &other) {
        boost::swap(uri_, other.uri_);
        boost::swap(uri_parts_, other.uri_parts_);
        boost::swap(is_valid_, other.is_valid_);
    }

    const_range_type scheme_range() const {
        using boost::fusion::at_c;
        return const_range_type(at_c<0>(at_c<0>(uri_parts_)),
                                at_c<1>(at_c<0>(uri_parts_)));
    }

    const_range_type user_info_range() const {
        using boost::fusion::at_c;
        return const_range_type(at_c<0>(at_c<1>(uri_parts_)),
                                at_c<1>(at_c<1>(uri_parts_)));
    }

    const_range_type host_range() const {
        using boost::fusion::at_c;
        return const_range_type(at_c<0>(at_c<2>(uri_parts_)),
                                at_c<1>(at_c<2>(uri_parts_)));
    }

    const_range_type port_range() const {
        using boost::fusion::at_c;
        return const_range_type(at_c<0>(at_c<3>(uri_parts_)),
                                at_c<1>(at_c<3>(uri_parts_)));
    }

    const_range_type path_range() const {
        using boost::fusion::at_c;
        return const_range_type(at_c<0>(at_c<4>(uri_parts_)),
                                at_c<1>(at_c<4>(uri_parts_)));
    }

    const_range_type query_range() const {
        using boost::fusion::at_c;
        return const_range_type(at_c<0>(at_c<5>(uri_parts_)),
                                at_c<1>(at_c<5>(uri_parts_)));
    }

    const_range_type fragment_range() const {
        using boost::fusion::at_c;
        return const_range_type(at_c<0>(at_c<6>(uri_parts_)),
                                at_c<1>(at_c<6>(uri_parts_)));
    }

    string_type scheme() const {
        const_range_type range = scheme_range();
        return string_type(boost::begin(range), boost::end(range));
    }

    string_type user_info() const {
        const_range_type range = user_info_range();
        return string_type(boost::begin(range), boost::end(range));
    }

    string_type host() const {
        const_range_type range = host_range();
        return string_type(boost::begin(range), boost::end(range));
    }

    string_type port() const {
        const_range_type range = port_range();
        return string_type(boost::begin(range), boost::end(range));
    }

    string_type path() const {
        const_range_type range = path_range();
        return string_type(boost::begin(range), boost::end(range));
    }

    string_type query() const {
        const_range_type range = query_range();
        return string_type(boost::begin(range), boost::end(range));
    }

    string_type fragment() const {
        const_range_type range = fragment_range();
        return string_type(boost::begin(range), boost::end(range));
    }

    string_type to_string() const {
        return uri_;
    }

    bool is_valid() const {
        return is_valid_;
    }

private:

    void parse();

    string_type uri_;
    details::uri_parts<Tag> uri_parts_;
    bool is_valid_;

};
} // namespace uri
} // namespace network
} // namespace boost


# include <boost/config/warning_disable.hpp>
# include <boost/spirit/include/qi.hpp>
# include <boost/spirit/include/phoenix_operator.hpp>
# include <boost/spirit/include/version.hpp>
# include <boost/spirit/repository/include/qi_iter_pos.hpp>


namespace boost {
namespace network {
namespace uri {
namespace details {
namespace qi = boost::spirit::qi;

template <
    class Tag,
    typename Iterator
    >
struct uri_grammar : qi::grammar<Iterator, uri_parts<Tag>()> {

    uri_grammar() : uri_grammar::base_type(start, "uri") {
        using boost::spirit::repository::qi::iter_pos;

        // // gen-delims = ":" / "/" / "?" / "#" / "[" / "]" / "@"
        // gen_delims %= qi::char_(":/?#[]@");
        // // sub-delims = "!" / "$" / "&" / "'" / "(" / ")" / "*" / "+" / "," / ";" / "="
        // sub_delims %= qi::char_("!$&'()*+,;=");
        // // reserved = gen-delims / sub-delims
        // reserved %= gen_delims | sub_delims;
        // // unreserved = ALPHA / DIGIT / "-" / "." / "_" / "~"
        // unreserved %= qi::alnum | qi::char_("-._~");
        // // pct-encoded = "%" HEXDIG HEXDIG
        // pct_encoded %= qi::char_("%") >> qi::repeat(2)[qi::xdigit];

        // // pchar = unreserved / pct-encoded / sub-delims / ":" / "@"
        // pchar %= qi::raw[
        //     unreserved | pct_encoded | sub_delims | qi::char_(":@")
        //     ];

        // // segment = *pchar
        // segment %= qi::raw[*pchar];
        // // segment-nz = 1*pchar
        // segment_nz %= qi::raw[+pchar];
        // // segment-nz-nc = 1*( unreserved / pct-encoded / sub-delims / "@" )
        // segment_nz_nc %= qi::raw[
        //     +(unreserved | pct_encoded | sub_delims | qi::char_("@"))
        //     ];
        // // path-abempty  = *( "/" segment )
        // path_abempty %= qi::raw[*(qi::char_("/") >> segment)];
        // // path-absolute = "/" [ segment-nz *( "/" segment ) ]
        // path_absolute %= qi::raw[
        //     qi::char_("/")
        //     >>  -(segment_nz >> *(qi::char_("/") >> segment))
        //     ];
        // // path-rootless = segment-nz *( "/" segment )
        // path_rootless %= qi::raw[
        //     segment_nz >> *(qi::char_("/") >> segment)
        //     ];
        // // path-empty = 0<pchar>
        // path_empty %= qi::eps;

        // // scheme = ALPHA *( ALPHA / DIGIT / "+" / "-" / "." )
        // scheme %= qi::alpha >> *(qi::alnum | qi::char_("+.-"));

        // // user_info = *( unreserved / pct-encoded / sub-delims / ":" )
        // user_info %= qi::raw[
        //     *(unreserved | pct_encoded | sub_delims | qi::char_(":"))
        //     ];

        // // dec-octet = DIGIT / %x31-39 DIGIT / "1" 2DIGIT / "2" %x30-34 DIGIT / "25" %x30-35
        // dec_octet %=
        //     !(qi::lit('0') >> qi::digit)
        //     >>  qi::raw[
        //         qi::uint_parser<boost::uint8_t, 10, 1, 3>()
        //         ];
        // // IPv4address = dec-octet "." dec-octet "." dec-octet "." dec-octet
        // ipv4address %= qi::raw[
        //     dec_octet >> qi::repeat(3)[qi::lit('.') >> dec_octet]
        //     ];
        // // reg-name = *( unreserved / pct-encoded / sub-delims )
        // reg_name %= qi::raw[
        //     *(unreserved | pct_encoded | sub_delims)
        //     ];
        // // TODO, host = IP-literal / IPv4address / reg-name
        // host %= ipv4address | reg_name;

        // // port %= qi::ushort_;

        // // query = *( pchar / "/" / "?" )
        // query %= qi::raw[*(pchar | qi::char_("/?"))];
        // // fragment = *( pchar / "/" / "?" )
        // fragment %= qi::raw[*(pchar | qi::char_("/?"))];

        // // hier-part = "//" authority path-abempty / path-absolute / path-rootless / path-empty
        // // authority = [ userinfo "@" ] host [ ":" port ]
        // hier_part %=
        //     (
        //         "//"
        //         >>  -(user_info >> '@')
        //         >>  host
        //         >> -(':' >> port)
        //         // >>  -(':' >> qi::ushort_)
        //         >>  path_abempty
        //         )
        //     |
        //     (
        //         qi::attr(std::string())
        //         >>  qi::attr(std::string())
        //         >>  qi::attr(std::string())
        //         >>  (
        //             path_absolute
        //             |   path_rootless
        //             |   path_empty
        //             )
        //         );

        // start %=
        //     iter_pos
        //     >> iter_pos
        //     >> iter_pos
        //     >> iter_pos
        //     >> iter_pos
        //     >> iter_pos
        //     >> qi::omit[scheme] >> ':'
        //     >> hier_part
        //     // >>  -('?' >> qi::omit[query])
        //     // >>  -('#' >> qi::omit[fragment])
        //     ;
    }

    qi::rule<Iterator, typename std::string::value_type()>
    gen_delims, sub_delims, reserved, unreserved;
    qi::rule<Iterator, std::string()>
    pct_encoded, pchar;

    qi::rule<Iterator, std::string()>
    segment, segment_nz, segment_nz_nc;
    qi::rule<Iterator, std::string()>
    path_abempty, path_absolute, path_rootless, path_empty;

    qi::rule<Iterator, std::string()>
    dec_octet, ipv4address, reg_name, host, port;

    qi::rule<Iterator, std::string()>
    scheme, user_info, query, fragment;

    qi::rule<Iterator, std::string()>
    hier_part;

    // actual uri parser
    qi::rule<Iterator, uri_parts<Tag>()> start;

};
} // namespace details


template <
    class Tag
    >
void basic_uri<Tag>::parse() {
    namespace qi = boost::spirit::qi;
    static details::uri_grammar<Tag, const_iterator_type> grammar;
    const_iterator_type first(boost::begin(uri_)), last(boost::end(uri_));
    is_valid_ = qi::parse(first, last, grammar, uri_parts_);
    is_valid_ = is_valid_ && (first == last);
}


template <
    class Tag
    >
inline
void swap(basic_uri<Tag> &lhs, basic_uri<Tag> &rhs) {
    lhs.swap(rhs);
}

template <
    class Tag
    >
inline
typename basic_uri<Tag>::string_type scheme(const basic_uri<Tag> &uri) {
    return uri.scheme();
}

template <
    class Tag
    >
inline
typename basic_uri<Tag>::string_type user_info(const basic_uri<Tag> &uri) {
    return uri.user_info();
}

template <
    class Tag
    >
inline
typename basic_uri<Tag>::string_type host(const basic_uri<Tag> &uri) {
    return uri.host();
}

template <
    class Tag
    >
inline
typename basic_uri<Tag>::string_type port(const basic_uri<Tag> &uri) {
    return uri.port();
}

template <
    class Tag
    >
inline
typename basic_uri<Tag>::string_type path(const basic_uri<Tag> &uri) {
    return uri.path();
}

template <
    class Tag
    >
inline
typename basic_uri<Tag>::string_type query(const basic_uri<Tag> &uri) {
    return uri.query();
}

template <
    class Tag
    >
inline
typename basic_uri<Tag>::string_type fragment(const basic_uri<Tag> &uri) {
    return uri.fragment();
}

template <
    class Tag
    >
inline
bool is_valid(const basic_uri<Tag> &uri) {
    return uri.is_valid();
}

template <
    class Tag
    >
inline
bool operator == (const basic_uri<Tag> &lhs, const basic_uri<Tag> &rhs) {
    return lhs.to_string() == rhs.to_string();
}
} // namespace uri
} // namespace network
} // namespace boost


#endif // __BOOST_NETWORK_URI_INC__
