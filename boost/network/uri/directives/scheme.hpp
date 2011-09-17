#ifndef __BOOST_NETWORK_URI_DIRECTIVES_SCHEME_INC__
# define __BOOST_NETWORK_URI_DIRECTIVES_SCHEME_INC__


# include <boost/range/begin.hpp>
# include <boost/range/end.hpp>


namespace boost {
namespace network {
namespace uri {
struct scheme_directive {

    explicit scheme_directive(const std::string &scheme)
        : scheme(scheme)
    {}

    void operator () (uri &uri_) const {
        static const char separator[] = {':', '/', '/'};
        uri_.append(scheme);
        uri_.append(boost::begin(separator), boost::end(separator));
    }

    std::string scheme;

};

inline
scheme_directive scheme(const std::string &scheme) {
    return scheme_directive(scheme);
}
} // namespace uri
} // namespace network
} // namespace boost


#endif // __BOOST_NETWORK_URI_DIRECTIVES_SCHEME_INC__
