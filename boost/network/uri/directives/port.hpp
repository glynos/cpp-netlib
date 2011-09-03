#ifndef __BOOST_NETWORK_URI_DIRECTIVES_PORT_INC__
# define __BOOST_NETWORK_URI_DIRECTIVES_PORT_INC__


# include <boost/utility/enable_if.hpp>
# include <boost/type_traits/is_integral.hpp>
# include <boost/cstdint.hpp>
# include <boost/range/begin.hpp>
# include <boost/range/end.hpp>


namespace boost {
namespace network {
namespace uri {
template <
    class ValueType
    >
struct port_directive {

    explicit port_directive(const ValueType &value)
        : value(value)
    {}

    template <
        class Tag
      , template <class> class Uri
        >
    void operator () (Uri<Tag> &uri) const {
        static const char separator[] = {':'};
        uri.append(boost::begin(separator), boost::end(separator));
        uri.append(value);
    }

    const ValueType &value;

};


struct port_directive_us {

    explicit port_directive_us(boost::uint16_t value)
        : value(value)
    {}

    template <
        class Tag
      , template <class> class Uri
        >
    void operator () (Uri<Tag> &uri) const {
        static const char separator[] = {':'};
        uri.append(boost::begin(separator), boost::end(separator));
        typename string<Tag>::type port = boost::lexical_cast<typename string<Tag>::type>(value);
        uri.append(port);
    }

    boost::uint16_t value;

};


template <
    class T
    >
inline
port_directive<T> port(const T &value,
                       typename boost::disable_if<typename boost::is_integral<T>::type>::type * = 0)  {
    return port_directive<T>(value);
}

inline
port_directive_us port(boost::uint16_t value) {
    return port_directive_us(value);
}
} // namespace uri
} // namespace network
} // namespace boost


#endif // __BOOST_NETWORK_URI_DIRECTIVES_PORT_INC__
