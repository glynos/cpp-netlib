// Copyright 2010 Dean Michael Berris.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>
#include <boost/network/include/http/client.hpp>
#include "client_types.hpp"

namespace net = boost::network;
namespace http = boost::network::http;

TYPED_TEST_CASE(HTTPClientTest, ClientTypes);

TYPED_TEST(HTTPClientTest, GetTest) {
  using client = TypeParam;
  typename client::request request("http://cpp-netlib.org/");
  client client_;
  typename client::response response;
  ASSERT_NO_THROW(response = client_.get(request));
  try {
    auto data = body(response);
    std::cout << data;
  } catch (...) {
    FAIL() << "Caught exception while retrieving body from GET request";
  }
  EXPECT_EQ("HTTP/1.", response.version().substr(0, 7));
  EXPECT_TRUE(response.status() == 200u ||
              (response.status() >= 300 && response.status() < 400));
}

#ifdef BOOST_NETWORK_ENABLE_HTTPS

TYPED_TEST(HTTPClientTest, GetHTTPSTest) {
  using client = TypeParam;
  typename client::request request("https://www.github.com/");
  client client_;
  typename client::response response = client_.get(request);
  EXPECT_TRUE(response.status() == 200 ||
              (response.status() >= 300 && response.status() < 400));
  try {
    auto data = body(response);
    std::cout << data;
  } catch (...) {
    FAIL() << "Caught exception while retrieving body from GET request";
  }
}

#endif

TYPED_TEST(HTTPClientTest, TemporaryClientObjectTest) {
  using client = TypeParam;
  typename client::request request("http://cpp-netlib.org/");
  typename client::response response;
  ASSERT_NO_THROW(response = client().get(request));
  auto range = headers(response);
  ASSERT_TRUE(!boost::empty(range));
  try {
    auto data = body(response);
    std::cout << data;
  } catch (...) {
    FAIL() << "Caught exception while retrieving body from GET request";
  }
  EXPECT_EQ("HTTP/1.", response.version().substr(0, 7));
  EXPECT_TRUE(response.status() == 200u ||
              (response.status() >= 300 && response.status() < 400));
}
