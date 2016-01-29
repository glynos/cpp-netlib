
// Copyright 2010 Dean Michael Berris.
// Copyright 2016 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>
#include <cstdlib>
#include <boost/network/include/http/client.hpp>
#include "client_types.hpp"
#include "http_test_server.hpp"

class LocalServerEnvironment : public ::testing::Environment {
 public:
  void SetUp() override {
    if (!server.start()) {
      std::cout << "Failed to start HTTP server for test!" << std::endl;
      std::abort();
    }
  }

  void TearDown() override {
    if (!server.stop()) {
      std::cout << "Failed to stop HTTP server for test!" << std::endl;
      std::abort();
    }
  }

  http_test_server server;
};

auto* local_env =
    ::testing::AddGlobalTestEnvironment(new LocalServerEnvironment);

TYPED_TEST_CASE(HTTPClientTest, ClientTypes);

TYPED_TEST(HTTPClientTest, GetTimeoutTest) {
  using client = TypeParam;
  typename client::request request("http://localhost:12121/");
  typename client::response response_;
  client client_;
  std::uint16_t port_ = port(request);
  typename client::response::string_type temp;
  EXPECT_EQ(12121, port_);
  EXPECT_THROW(response_ = client_.get(request); temp = body(response_);
               , std::exception);
}

TYPED_TEST(HTTPClientTest, GetTimeoutWithOptionsTest) {
  using client = TypeParam;
  typename client::request request("http://localhost:8000/cgi-bin/sleep.py?3");
  typename client::response response;
  typename client::options options;
  client client_(options.timeout(1));
  typename client::response::string_type temp;
  EXPECT_THROW(response = client_.get(request); temp = body(response);
               , std::exception);
}

#ifdef BOOST_NETWORK_ENABLE_HTTPS

TYPED_TEST(HTTPClientTest, HTTPSGetTimeoutWithOptionsTest) {
  using client = TypeParam;
  typename client::request request("https://localhost:8000/cgi-bin/sleep.py?3");
  typename client::response response;
  typename client::options options;
  client client_(options.timeout(1));
  typename client::response::string_type temp;
  EXPECT_THROW(response = client_.get(request); temp = body(response);
               , std::exception);
}

#endif
