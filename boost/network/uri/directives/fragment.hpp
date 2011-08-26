#ifndef __BOOST_NETWORK_URI_DIRECTIVES_FRAGMENT_INC__
# define __BOOST_NETWORK_URI_DIRECTIVES_FRAGMENT_INC__


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
struct fragment_directive {

    explicit fragment_directive(const ValueType &value)
        : value(value)
    {}

    template <
        class Tag
      , template <class> class Uri
        >
    typename enable_if<is_pod<Tag>, void>::type
    operator () (Uri<Tag> &uri) const {
        static const char separator[] = {'#'};
        uri.append(boost::begin(separator), boost::end(separator));
        uri.append(value);
    }

    template <
        class Tag
      , template <class> class Uri
        >
    typename enable_if<mpl::not_<is_pod<Tag> >, void>::type
    operator () (Uri<Tag> &uri) const {
        static const char separator[] = {'#'};
        uri.append(boost::begin(separator), boost::end(separator));
        uri.append(value);
    }

    const ValueType &value;

};

template <
    class T
    >
inline
fragment_directive<T> fragment(const T &value)  {
    return fragment_directive<T>(value);
}
} // namespace uri
} // namespace network
} // namespace boost


#endif // __BOOST_NETWORK_URI_DIRECTIVES_FRAGMENT_INC__
