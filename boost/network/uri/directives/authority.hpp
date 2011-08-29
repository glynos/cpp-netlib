#ifndef __BOOST_NETWORK_URI_DIRECTIVES_AUTHORITY_INC__
# define __BOOST_NETWORK_URI_DIRECTIVES_AUTHORITY_INC__


# include <boost/range/as_literal.hpp>



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
    void operator () (Uri<Tag> &uri) const {
        uri.append(value);
    }

    const ValueType &value;

};

template <
    class T
    >
inline
authority_directive<T> authority(const T &value) {
    return authority_directive<T>(value);
}
} // namespace uri
} // namespace network
} // namespace boost


#endif // __BOOST_NETWORK_URI_DIRECTIVES_AUTHORITY_INC__
