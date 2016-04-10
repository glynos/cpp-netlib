// Copyright 2016 Glyn Matthews.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#include <gtest/gtest.h>
// #include <boost/network/protocol/http/client/response.hpp>
#include <boost/network/protocol/http/client.hpp>


using client_response = boost::network::http::client::response;


TEST(http_client_response, construct) {
  ASSERT_NO_THROW(client_response());
}
