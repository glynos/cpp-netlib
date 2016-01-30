
// Copyright 2010 Dean Michael Berris.
// Copyright 2016 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>
#include <boost/network/include/http/client.hpp>
#include "client_types.hpp"

namespace net = boost::network;
namespace http = boost::network::http;

TYPED_TEST_CASE(HTTPClientTest, ClientTypes);

TYPED_TEST(HTTPClientTest, GetDifferentPort) {
  TypeParam client;
  typename TypeParam::request r("http://www.boost.org:80/");
  auto response_ = client.get(r);
  auto range = headers(response_)["Content-Type"];
  EXPECT_TRUE(std::begin(range) != std::end(range));
  EXPECT_NE(0, body(response_).size());
}
