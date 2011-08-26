#ifndef __BOOST_NETWORK_URI_DIRECTIVES_QUERY_INC__
# define __BOOST_NETWORK_URI_DIRECTIVES_QUERY_INC__


# include <boost/network/support/is_pod.hpp>
# include <boost/utility/enable_if.hpp>
# include <boost/mpl/if.hpp>
# include <boost/mpl/or.hpp>


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
    typename enable_if<is_pod<Tag>, void>::type
    operator () (Uri<Tag> &uri) const {
        static const char separator[] = {'?'};
        uri.append(boost::begin(separator), boost::end(separator));
        uri.append(value);
    }

    template <
        class Tag
      , template <class> class Uri
        >
    typename enable_if<mpl::not_<is_pod<Tag> >, void>::type
    operator () (Uri<Tag> &uri) const {
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
    typename enable_if<is_pod<Tag>, void>::type
    operator () (Uri<Tag> &uri) const {
        static const char separator_1[] = {'?'};
        static const char separator_2[] = {'='};
        static const char separator_3[] = {';'};
        if (!uri.query_range())
        {
            uri.append(boost::begin(separator_1), boost::end(separator_1));
        }
        else
        {
            uri.append(boost::begin(separator_3), boost::end(separator_3));
        }
        uri.append(key);
        uri.append(boost::begin(separator_2), boost::end(separator_2));
        typename string<Tag>::type encoded_value;
        encode(boost::begin(value), boost::end(value), std::back_inserter(encoded_value));
        uri.append(encoded_value);
    }

    template <
        class Tag
      , template <class> class Uri
        >
    typename enable_if<mpl::not_<is_pod<Tag> >, void>::type
    operator () (Uri<Tag> &uri) const {
        static const char separator_1[] = {'?'};
        static const char separator_2[] = {'='};
        static const char separator_3[] = {';'};
        if (!uri.query_range())
        {
            uri.append(boost::begin(separator_1), boost::end(separator_1));
        }
        else
        {
            uri.append(boost::begin(separator_3), boost::end(separator_3));
        }
        uri.append(key);
        uri.append(boost::begin(separator_2), boost::end(separator_2));
        uri.append(value);
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
