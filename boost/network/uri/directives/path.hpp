#ifndef __BOOST_NETWORK_URI_DIRECTIVES_PATH_INC__
# define __BOOST_NETWORK_URI_DIRECTIVES_PATH_INC__


# include <boost/network/uri/encode.hpp>
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
struct path_directive {

    explicit path_directive(const ValueType &value)
        : value(value)
    {}

    template <
        class Tag
      , template <class> class Uri
        >
    typename enable_if<is_pod<Tag>, void>::type
    operator () (Uri<Tag> &uri) const {
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
        typename string<Tag>::type encoded_value;
        encode(boost::begin(value), boost::end(value), std::back_inserter(encoded_value));
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
} // namespace uri
} // namespace network
} // namespace boost


#endif // __BOOST_NETWORK_URI_DIRECTIVES_PATH_INC__
