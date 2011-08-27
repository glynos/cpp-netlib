#ifndef __BOOST_NETWORK_URI_DIRECTIVES_SCHEME_INC__
# define __BOOST_NETWORK_URI_DIRECTIVES_SCHEME_INC__


# include <boost/range/begin.hpp>
# include <boost/range/end.hpp>


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
    void operator () (Uri<Tag> &uri) const {
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

namespace schemes {
struct scheme_helper {

    scheme_helper(const char *scheme)
        : scheme(scheme) {

    }

    template <
        class Tag
      , template <class> class Uri
        >
    void operator () (Uri<Tag> &uri) const {
        static const char separator[] = {':', '/', '/'};
        uri.append(boost::begin(scheme), boost::end(scheme));
        uri.append(boost::begin(separator), boost::end(separator));
    }

    std::string scheme;

};

//static scheme_helper http("http");
//static scheme_helper https("https");
//static scheme_helper ftp("ftp");
//static scheme_helper mailto("mailto");
} // namespace schemes
} // namespace uri
} // namespace network
} // namespace boost


#endif // __BOOST_NETWORK_URI_DIRECTIVES_SCHEME_INC__
