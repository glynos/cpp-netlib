#ifndef BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_MODIFIERS_URI_HPP_20100621
#define BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_MODIFIERS_URI_HPP_20100621

// Copyright 2010 (c) Dean Michael Berris
// Copyright 2010 (c) Sinefunc, Inc.
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/protocol/http/request/request_base.hpp>

namespace boost { namespace network { namespace http {

inline void uri(request_base & request, std::string const & value) {
  request.set_uri(value);
}

inline void uri(request_base & request, ::network::uri const & value) {
  request.set_uri(value);
}

} // namespace http

} // namespace network

} // namespace boost

#endif // BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_MODIFIERS_URI_HPP_20100621
