// Copyright 2010 Dean Michael Berris.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>
#include <boost/network/include/http/client.hpp>
#include "client_types.hpp"

namespace net = boost::network;
namespace http = boost::network::http;
using tclock = std::chrono::high_resolution_clock;

TYPED_TEST_CASE(HTTPClientTest, ClientTypes);

TYPED_TEST(HTTPClientTest, GetTest) {
  using client = TypeParam;
  typename client::request request("http://cpp-netlib.org/");
  client client_;
  auto response = client_.get(request);
  while (!http::ready(response)) {
	  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
  auto t0 = tclock::now();
  auto data = body(response);
  auto t1 = tclock::now();
  EXPECT_TRUE(response.status() == 200u ||
              (response.status() >= 300 && response.status() < 400));
  EXPECT_TRUE(data.size() > 0);

  // XXX we should find a better way to check if `ready()` has done his job
  namespace c = std::chrono;
  EXPECT_TRUE(c::duration_cast<c::milliseconds>(t1-t0).count() < 1);
}
