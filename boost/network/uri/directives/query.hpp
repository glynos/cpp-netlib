#ifndef __BOOST_NETWORK_URI_DIRECTIVES_QUERY_INC__
# define __BOOST_NETWORK_URI_DIRECTIVES_QUERY_INC__


# include <boost/network/uri/encode.hpp>
# include <boost/range/begin.hpp>
# include <boost/range/end.hpp>


namespace boost {
namespace network {
namespace uri {
template <
    class ValueType
    >
struct query_directive {

    explicit query_directive(const ValueType &value)
        : value(value)
    {}

    template <
        class Tag
      , template <class> class Uri
        >
    void operator () (Uri<Tag> &uri) const {
        typename string<Tag>::type encoded_value;
        static const char separator[] = {'?'};
        uri.append(boost::begin(separator), boost::end(separator));
        uri.append(value);
    }

    const ValueType &value;

};

template <
    class T
    >
inline
query_directive<T> query(const T &value)  {
    return query_directive<T>(value);
}

template <
    class KeyType
  , class ValueType
    >
struct query_key_value_directive {

    query_key_value_directive(const KeyType &key, const ValueType &value)
        : key(key), value(value)
    {}

    template <
        class Tag
      , template <class> class Uri
        >
    void operator () (Uri<Tag> &uri) const {
        typename string<Tag>::type encoded_key, encoded_value;
        static const char qmark[] = {'?'};
        static const char equal[] = {'='};
        static const char scolon[] = {';'};
        if (!uri.query_range())
        {
            uri.append(boost::begin(qmark), boost::end(qmark));
        }
        else
        {
            uri.append(boost::begin(scolon), boost::end(scolon));
        }
        encode(boost::as_literal(key), std::back_inserter(encoded_key));
        uri.append(encoded_key);
        uri.append(boost::begin(equal), boost::end(equal));
        encode(boost::as_literal(value), std::back_inserter(encoded_value));
        uri.append(encoded_value);
    }

    const KeyType &key;
    const ValueType &value;

};

template <
    class Key
  , class Value
    >
inline
query_key_value_directive<Key, Value> query(const Key &key, const Value &value)  {
    return query_key_value_directive<Key, Value>(key, value);
}
} // namespace uri
} // namespace network
} // namespace boost


#endif // __BOOST_NETWORK_URI_DIRECTIVES_QUERY_INC__
