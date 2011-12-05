#ifndef __BOOST_NETWORK_URI_URI_IO_INC__
# define __BOOST_NETWORK_URI_URI_IO_INC__


# include <boost/network/uri/uri.hpp>


namespace boost {
namespace network {
namespace uri {
template <
    class Tag
    >
inline
std::ostream &operator << (std::ostream &os, const basic_uri<Tag> &uri) {
    return os << uri.string();
}
} // namespace uri
} // namespace network
} // namespace boost


#endif // __BOOST_NETWORK_URI_URI_IO_INC__
