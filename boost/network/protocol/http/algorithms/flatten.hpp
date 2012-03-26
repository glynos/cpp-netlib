#ifndef BOOST_NETWORK_PROTOCOL_HTTP_ALGORITHMS_FLATTEN_HPP_20120326
#define BOOST_NETWORK_PROTOCOL_HTTP_ALGORITHMS_FLATTEN_HPP_20120326

// Copyright 2012 Dean Michael Berris <dberris@google.com>.
// Copyright 2012 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <vector>
#include <boost/asio/buffer.hpp>

namespace boost { namespace network { namespace http {

class response;

void flatten(response const &response_, std::vector<asio::const_buffer> &buffers);

}  // namespace http

}  // namespace network

}  // namespace boost

#endif  // BOOST_NETWORK_PROTOCOL_HTTP_ALGORITHMS_FLATTEN_HPP_20120326
