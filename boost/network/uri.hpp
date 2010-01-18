#ifndef BOOST_NETWORK_URL_HPP_
#define BOOST_NETWORK_URL_HPP_

// Copyright 2009 Dean Michael Berris.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/tags.hpp>
#include <boost/network/traits/string.hpp>
#include <boost/network/uri/basic_uri.hpp>
// TODO, #include <boost/network/uri/http/uri.hpp>

namespace boost { namespace network { namespace uri {

    typedef basic_uri<boost::network::tags::default_string> uri;
    typedef basic_uri<boost::network::tags::default_wstring> wuri;

    /* TODO, namespace http {
        typedef basic_uri<tags::http_default_8bit_tcp_resolve> uri;
    } */

} // namespace uri
} // namespace network
} // namespace boost

#endif

