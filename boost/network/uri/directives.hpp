#ifndef __BOOST_NETWORK_URI_DIRECTIVES_INC__
# define __BOOST_NETWORK_URI_DIRECTIVES_INC__


# include <boost/network/uri/uri.hpp>
# include <boost/network/uri/directives/scheme.hpp>
# include <boost/network/uri/directives/user_info.hpp>
# include <boost/network/uri/directives/host.hpp>
# include <boost/network/uri/directives/port.hpp>
# include <boost/network/uri/directives/authority.hpp>
# include <boost/network/uri/directives/path.hpp>
# include <boost/network/uri/directives/query.hpp>
# include <boost/network/uri/directives/fragment.hpp>
# include <boost/network/support/is_pod.hpp>
# include <boost/utility/enable_if.hpp>
# include <boost/mpl/if.hpp>
# include <boost/mpl/or.hpp>


namespace boost {
namespace network {
namespace uri {
template <
    class Tag
  , class Directive
    >
inline
basic_uri<Tag> &operator << (basic_uri<Tag> &uri, const Directive &directive) {
    directive(uri);
    return uri;
}
} // namespace uri
} // namespace network
} // namespace boost


#endif // __BOOST_NETWORK_URI_DIRECTIVES_INC__
