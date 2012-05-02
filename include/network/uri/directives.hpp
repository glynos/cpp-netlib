//            Copyright (c) Glyn Matthews 2011, 2012.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#ifndef __BOOST_NETWORK_URI_DIRECTIVES_INC__
# define __BOOST_NETWORK_URI_DIRECTIVES_INC__


# include <network/uri/uri.hpp>


namespace network {
inline
uri &operator << (uri &uri_, const uri &root_uri) {
    if (boost::empty(uri_) && valid(root_uri)) {
        uri_.append(boost::begin(root_uri), boost::end(root_uri));
    }
    return uri_;
}

template <
    class Directive
    >
inline
uri &operator << (uri &uri_, const Directive &directive) {
    directive(uri_);
    return uri_;
}
} // namespace network


# include <network/uri/directives/scheme.hpp>
# include <network/uri/directives/user_info.hpp>
# include <network/uri/directives/host.hpp>
# include <network/uri/directives/port.hpp>
# include <network/uri/directives/authority.hpp>
# include <network/uri/directives/path.hpp>
# include <network/uri/directives/query.hpp>
# include <network/uri/directives/fragment.hpp>


#endif // __BOOST_NETWORK_URI_DIRECTIVES_INC__
