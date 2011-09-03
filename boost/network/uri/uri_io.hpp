#ifndef __BOOST_NETWORK_URI_URI_IO_INC__
# define __BOOST_NETWORK_URI_URI_IO_INC__


# include <boost/network/uri/uri.hpp>
# include <string>


namespace boost {
namespace network {
namespace uri {
inline
std::ostream &operator << (std::ostream &os, const basic_uri<std::string> &uri) {
    return os << uri.string();
}

inline
std::wostream &operator << (std::wostream &os, const basic_uri<std::wstring> &uri) {
    return os << uri.string();
}
} // namespace uri
} // namespace network
} // namespace boost


#endif // __BOOST_NETWORK_URI_URI_IO_INC__
