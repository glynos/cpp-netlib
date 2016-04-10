// Copyright 2016 Glyn Matthews.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#include <gtest/gtest.h>
// #include <boost/network/protocol/http/client/request.hpp>
#include <boost/network/protocol/http/client.hpp>


using client_request = boost::network::http::client::request;


TEST(http_client_request, construct_from_uri) {
  ASSERT_NO_THROW(client_request("http://cpp-netlib.org/"));
}

TEST(http_client_request, construct_from_invalid_uri) {
  ASSERT_THROW(client_request("I am not a valid URI"), std::exception);
}
