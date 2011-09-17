#ifndef __BOOST_NETWORK_URI_DIRECTIVES_QUERY_INC__
# define __BOOST_NETWORK_URI_DIRECTIVES_QUERY_INC__


# include <boost/network/uri/encode.hpp>
# include <boost/range/begin.hpp>
# include <boost/range/end.hpp>


namespace boost {
namespace network {
namespace uri {
struct query_directive {

    explicit query_directive(const std::string &query)
        : query(query)
    {}

    void operator () (uri &uri_) const {
        std::string encoded_query;
        static const char separator[] = {'?'};
        uri_.append(boost::begin(separator), boost::end(separator));
        uri_.append(query);
    }

    std::string query;

};

inline
query_directive query(const std::string &query)  {
    return query_directive(query);
}

struct query_key_query_directive {

    query_key_query_directive(const std::string &key, const std::string &query)
        : key(key), query(query)
    {}

    void operator () (uri &uri_) const {
        std::string encoded_key, encoded_query;
        static const char qmark[] = {'?'};
        static const char equal[] = {'='};
        static const char scolon[] = {';'};
        if (!uri_.query_range())
        {
            uri_.append(boost::begin(qmark), boost::end(qmark));
        }
        else
        {
            uri_.append(boost::begin(scolon), boost::end(scolon));
        }
        encode(key, std::back_inserter(encoded_key));
        uri_.append(encoded_key);
        uri_.append(boost::begin(equal), boost::end(equal));
        encode(query, std::back_inserter(encoded_query));
        uri_.append(encoded_query);
    }

    std::string key;
    std::string query;

};

inline
query_key_query_directive query(const std::string &key, const std::string &query)  {
    return query_key_query_directive(key, query);
}
} // namespace uri
} // namespace network
} // namespace boost


#endif // __BOOST_NETWORK_URI_DIRECTIVES_QUERY_INC__
