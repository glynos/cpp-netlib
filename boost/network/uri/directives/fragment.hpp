#ifndef __BOOST_NETWORK_URI_DIRECTIVES_FRAGMENT_INC__
# define __BOOST_NETWORK_URI_DIRECTIVES_FRAGMENT_INC__


# include <boost/network/uri/encode.hpp>
# include <boost/range/begin.hpp>
# include <boost/range/end.hpp>


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
        class String
      , template <class> class Uri
        >
    void operator () (Uri<String> &uri) const {
        String encoded_value;
        static const char separator[] = {'#'};
        uri.append(boost::begin(separator), boost::end(separator));
        encode(boost::as_literal(value), std::back_inserter(encoded_value));
        uri.append(encoded_value);
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
