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

    template <
        class Uri
        >
    void operator () (Uri &uri) const {
        uri.append(scheme);
        uri.append("://");
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
