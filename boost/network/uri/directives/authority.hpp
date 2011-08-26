#ifndef __BOOST_NETWORK_URI_DIRECTIVES_AUTHORITY_INC__
# define __BOOST_NETWORK_URI_DIRECTIVES_AUTHORITY_INC__


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
struct authority_directive {

    explicit authority_directive(const ValueType &value)
        : value(value)
    {}

    template <
        class Tag
      , template <class> class Uri
        >
    typename enable_if<is_pod<Tag>, void>::type
    operator () (Uri<Tag> &uri) const {
        uri.append(value);
    }

    template <
        class Tag
      , template <class> class Uri
        >
    typename enable_if<mpl::not_<is_pod<Tag> >, void>::type
    operator () (Uri<Tag> &uri) const {
        uri.append(value);
    }

    const ValueType &value;

};

template <
    class T
    >
inline
authority_directive<T> authority(const T &value)  {
    return authority_directive<T>(value);
}
} // namespace uri
} // namespace network
} // namespace boost


#endif // __BOOST_NETWORK_URI_DIRECTIVES_AUTHORITY_INC__
