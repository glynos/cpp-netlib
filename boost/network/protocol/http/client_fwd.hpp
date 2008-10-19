
//          Copyright Dean Michael Berris 2007-2008.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef __NETWORK_PROTOCOL_HTTP_CLIENT_20080923_1_HPP__
#define __NETWORK_PROTOCOL_HTTP_CLIENT_20080923_1_HPP__

#ifndef BOOST_NETLIB_VERSION
#define BOOST_NETLIB_VERSION "0.3"
#endif

#include <boost/network/protocol/http/tags.hpp>

namespace boost { namespace network { namespace http {

    //! Forward declaration of basic_client template.
    template <class tag, unsigned version_major, unsigned version_minor>
        class basic_client;

    typedef basic_client<http::message_tag, 1, 0> client;

} // namespace http

} // namespace network

} // namespace boost

#endif // __NETWORK_PROTOCOL_HTTP_CLIENT_20080923_1_HPP__

