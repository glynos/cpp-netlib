#ifndef __BOOST_NETWORK_URI_DIRECTIVES_PATH_INC__
# define __BOOST_NETWORK_URI_DIRECTIVES_PATH_INC__


# include <boost/network/uri/encode.hpp>
# include <boost/range/as_literal.hpp>


namespace boost {
namespace network {
namespace uri {
template <
    class ValueType
    >
struct path_directive {

    explicit path_directive(const ValueType &value)
        : value(value)
    {}

    template <
        class Tag
      , template <class> class Uri
        >
    void operator () (Uri<Tag> &uri) const {
        (*this)(boost::as_literal(value), uri);
    }

    template <
        class Rng
      , class Tag
      , template <class> class Uri
        >
    void operator () (const Rng &rng, Uri<Tag> &uri) const {
        uri.append(boost::begin(rng), boost::end(rng));
    }

    const ValueType &value;

};

template <
    class ValueType
    >
struct encoded_path_directive {

    explicit encoded_path_directive(const ValueType &value)
        : value(value)
    {}

    template <
        class StringT
      , template <class> class Uri
        >
    void operator () (Uri<StringT> &uri) const {
        StringT encoded_value;
        encode(boost::as_literal(value), std::back_inserter(encoded_value));
        uri.append(encoded_value);
    }

    const ValueType &value;

};

template <
    class T
    >
inline
path_directive<T> path(const T &value)  {
    return path_directive<T>(value);
}

template <
    class T
    >
inline
encoded_path_directive<T> encoded_path(const T &value)  {
    return encoded_path_directive<T>(value);
}
} // namespace uri
} // namespace network
} // namespace boost


#endif // __BOOST_NETWORK_URI_DIRECTIVES_PATH_INC__
