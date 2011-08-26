#ifndef __BOOST_NETWORK_URI_DIRECTIVES_SCHEME_INC__
# define __BOOST_NETWORK_URI_DIRECTIVES_SCHEME_INC__


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
struct scheme_directive {

    explicit scheme_directive(const ValueType &value)
        : value(value)
    {}

    template <
        class Tag
      , template <class> class Uri
        >
    typename enable_if<is_pod<Tag>, void>::type
    operator () (Uri<Tag> &uri) const {
        static const char separator[] = {':', '/', '/'};
        uri.append(value);
        uri.append(boost::begin(separator), boost::end(separator));
    }

    template <
        class Tag
      , template <class> class Uri
        >
    typename enable_if<mpl::not_<is_pod<Tag> >, void>::type
    operator () (Uri<Tag> &uri) const {
        static const char separator[] = {':', '/', '/'};
        uri.append(value);
        uri.append(boost::begin(separator), boost::end(separator));
    }

    const ValueType &value;

};

template <
    class T
    >
inline
scheme_directive<T> scheme(const T &value)  {
    return scheme_directive<T>(value);
}
} // namespace uri
} // namespace network
} // namespace boost


#endif // __BOOST_NETWORK_URI_DIRECTIVES_SCHEME_INC__
