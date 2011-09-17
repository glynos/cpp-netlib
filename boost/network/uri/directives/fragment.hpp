#ifndef __BOOST_NETWORK_URI_DIRECTIVES_FRAGMENT_INC__
# define __BOOST_NETWORK_URI_DIRECTIVES_FRAGMENT_INC__


# include <boost/network/uri/encode.hpp>
# include <boost/range/begin.hpp>
# include <boost/range/end.hpp>


namespace boost {
namespace network {
namespace uri {
struct fragment_directive {

    explicit fragment_directive(const std::string &fragment)
        : fragment(fragment)
    {}

    void operator () (uri &uri_) const {
        std::string encoded_fragment;
        static const char separator[] = {'#'};
        uri_.append(boost::begin(separator), boost::end(separator));
        encode(fragment, std::back_inserter(encoded_fragment));
        uri_.append(encoded_fragment);
    }

    std::string fragment;

};

inline
fragment_directive fragment(const std::string &fragment)  {
    return fragment_directive(fragment);
}
} // namespace uri
} // namespace network
} // namespace boost


#endif // __BOOST_NETWORK_URI_DIRECTIVES_FRAGMENT_INC__
