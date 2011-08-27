#ifndef __BOOST_NETWORK_URI_DIRECTIVES_HOST_INC__
# define __BOOST_NETWORK_URI_DIRECTIVES_HOST_INC__


# include <boost/range/begin.hpp>
# include <boost/range/end.hpp>


namespace boost {
namespace network {
namespace uri {
template <
    class ValueType
    >
struct host_directive {

    explicit host_directive(const ValueType &value)
        : value(value)
    {}

    template <
        class Tag
      , template <class> class Uri
        >
    void operator () (Uri<Tag> &uri) const {
        uri.append(value);
    }

    const ValueType &value;

};

template <
    class T
    >
inline
host_directive<T> host(const T &value)  {
    return host_directive<T>(value);
}
} // namespace uri
} // namespace network
} // namespace boost


#endif // __BOOST_NETWORK_URI_DIRECTIVES_HOST_INC__
