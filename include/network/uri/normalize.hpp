//            Copyright (c) Glyn Matthews 2012.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#ifndef __BOOST_NETWORK_URI_NORMALIZE_INC__
# define __BOOST_NETWORK_URI_NORMALIZE_INC__

# include <network/uri/uri.hpp>

namespace network {
uri::string_type normalize_path(const uri::const_range_type &path);

//uri normalize(const uri &uri_);
//
//uri::string_type normalize_scheme(const uri &uri_);
//
//uri::string_type normalize_user_info(const uri &uri_);
//
//uri::string_type normalize_host(const uri &uri_);
//
//uri::string_type normalize_port(const uri &uri_);
//
//uri::string_type normalize_path(const uri &uri_);
//
//uri::string_type normalize_fragment(const uri &uri_);
//
//uri::string_type normalize_query(const uri &uri_);
} // namespace network

#endif // __BOOST_NETWORK_URI_NORMALIZE_INC__
