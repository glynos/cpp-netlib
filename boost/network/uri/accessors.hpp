//            Copyright (c) Glyn Matthews 2011.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#ifndef __BOOST_NETWORK_URI_URI_ACCESSORS_INC__
# define __BOOST_NETWORK_URI_URI_ACCESSORS_INC__


# include <boost/network/uri/uri.hpp>
# include <boost/network/uri/encode.hpp>
# include <boost/network/uri/decode.hpp>
# include <boost/spirit/include/qi.hpp>
# include <boost/fusion/include/std_pair.hpp>


namespace boost {
namespace network {
namespace uri {
namespace details {
template <
    typename Uri,
    typename Map
    >
struct key_value_sequence
    : spirit::qi::grammar<typename Uri::const_iterator, Map()>
{
    typedef typename Uri::const_iterator const_iterator;

    key_value_sequence()
        : key_value_sequence::base_type(query)
    {
        query =  pair >> *((spirit::qi::lit(';') | '&') >> pair);
        pair  =  key >> -('=' >> value);
        key   =  spirit::qi::char_("a-zA-Z_") >> *spirit::qi::char_("a-zA-Z_0-9/%");
        value = +spirit::qi::char_("a-zA-Z_0-9/%");
    }

    spirit::qi::rule<const_iterator, Map()> query;
    spirit::qi::rule<const_iterator, std::pair<typename Uri::string_type, typename Uri::string_type>()> pair;
    spirit::qi::rule<const_iterator, typename Uri::string_type()> key, value;
};
} // namespace details

template <
    class String,
    class Map
    >
inline
Map &query_map(const basic_uri<String> &uri, Map &map) {
    typename basic_uri<String>::const_range_type range = uri.query_range();
    details::key_value_sequence<basic_uri<String>, Map> parser;
    spirit::qi::parse(boost::begin(range), boost::end(range), parser, map);
    return map;
}

template <
    class String
    >
String username(const basic_uri<String> &uri) {
    typename basic_uri<String>::const_range_type user_info_range = uri.user_info_range();
    typename basic_uri<String>::const_iterator it(boost::begin(user_info_range)), end(boost::end(user_info_range));
    for (; it != end; ++it) {
        if (*it == ':') {
            break;
        }
    }
    return String(boost::begin(user_info_range), it);
}

template <
    class String
    >
String password(const basic_uri<String> &uri) {
    typename basic_uri<String>::const_range_type user_info_range = uri.user_info_range();
    typename basic_uri<String>::const_iterator it(boost::begin(user_info_range)), end(boost::end(user_info_range));
    for (; it != end; ++it) {
        if (*it == ':') {
            ++it;
            break;
        }
    }
    return String(it, boost::end(user_info_range));
}

template <
    class String
    >
String decoded_path(const basic_uri<String> &uri) {
    typename basic_uri<String>::const_range_type path_range = uri.path_range();
    String decoded_path;
    decode(path_range, std::back_inserter(decoded_path));
    return decoded_path;
}

template <
    class String
    >
String decoded_query(const basic_uri<String> &uri) {
    typename basic_uri<String>::const_range_type query_range = uri.query_range();
    String decoded_query;
    decode(query_range, std::back_inserter(decoded_query));
    return decoded_query;
}

template <
    class String
    >
String decoded_fragment(const basic_uri<String> &uri) {
    typename basic_uri<String>::const_range_type fragment_range = uri.fragment_range();
    String decoded_fragment;
    decode(fragment_range, std::back_inserter(decoded_fragment));
    return decoded_fragment;
}
} // namespace uri
} // namespace network
} // namespace boost


#endif // __BOOST_NETWORK_URI_URI_ACCESSORS_INC__
