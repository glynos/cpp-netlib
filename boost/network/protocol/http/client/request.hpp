// Copyright 2016 Glyn Matthews.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#if !defined(BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_REQUEST_INC)
#define BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_REQUEST_INC

#include <boost/network/protocol/http/impl/request.hpp>

namespace boost {
namespace network {
namespace http {
using client_request = basic_request<tags::http_async_8bit_tcp_resolve>;
}  // namespace http
}  // namespace network
}  // namespace boost

#endif  // BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_REQUEST_INC
