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
    typename Map
    >
struct key_value_sequence
    : spirit::qi::grammar<uri::const_iterator, Map()>
{
    key_value_sequence()
        : key_value_sequence::base_type(query)
    {
        query =  pair >> *((spirit::qi::lit(';') | '&') >> pair);
        pair  =  key >> -('=' >> value);
        key   =  spirit::qi::char_("a-zA-Z_") >> *spirit::qi::char_("a-zA-Z_0-9/%");
        value = +spirit::qi::char_("a-zA-Z_0-9/%");
    }

    spirit::qi::rule<uri::const_iterator, Map()> query;
    spirit::qi::rule<uri::const_iterator, std::pair<std::string, std::string>()> pair;
    spirit::qi::rule<uri::const_iterator, typename std::string()> key, value;
};
} // namespace details

template <
    class Map
    >
inline
Map &query_map(const uri &uri_, Map &map) {
    std::string range = uri_.query();
    details::key_value_sequence<Map> parser;
    spirit::qi::parse(boost::begin(range), boost::end(range), parser, map);
    return map;
}

std::string username(const uri &uri_) {
    std::string user_info = uri_.user_info();
    uri::const_iterator it(boost::begin(user_info)), end(boost::end(user_info));
    for (; it != end; ++it) {
        if (*it == ':') {
            break;
        }
    }
    return std::string(boost::begin(user_info), it);
}

std::string password(const uri &uri_) {
    std::string user_info = uri_.user_info();
    uri::const_iterator it(boost::begin(user_info)), end(boost::end(user_info));
    for (; it != end; ++it) {
        if (*it == ':') {
            ++it;
            break;
        }
    }
    return std::string(it, boost::end(user_info));
}

std::string decoded_path(const uri &uri_) {
    std::string path = uri_.path();
    std::string decoded_path;
    decode(path, std::back_inserter(decoded_path));
    return decoded_path;
}

std::string decoded_query(const uri &uri_) {
    std::string query = uri_.query();
    std::string decoded_query;
    decode(query, std::back_inserter(decoded_query));
    return decoded_query;
}

std::string decoded_fragment(const uri &uri_) {
    std::string fragment = uri_.fragment();
    std::string decoded_fragment;
    decode(fragment, std::back_inserter(decoded_fragment));
    return decoded_fragment;
}
} // namespace uri
} // namespace network
} // namespace boost


#endif // __BOOST_NETWORK_URI_URI_ACCESSORS_INC__
