
// Copyright 2010 Dean Michael Berris.
// Copyright 2016 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>
#include <boost/network/message/directives/header.hpp>
#include <boost/network/protocol/http/algorithms/linearize.hpp>
#include <boost/network/protocol/http/request.hpp>
#include <iostream>
#include <iterator>
#include "tag_types.hpp"

namespace http = boost::network::http;
namespace tags = boost::network::http::tags;
namespace mpl = boost::mpl;
namespace net = boost::network;

template <class T>
class LinearizeTest : public ::testing::Test {};

TYPED_TEST_CASE(LinearizeTest, TagTypes);

TYPED_TEST(LinearizeTest, LinearizeRequest) {
  http::basic_request<TypeParam> request("http://www.boost.org");
  static char http_1_0_output[] =
      "GET / HTTP/1.0\r\n"
      "Host: www.boost.org\r\n"
      "Accept: */*\r\n"
      "Connection: Close\r\n"
      "\r\n";
  static char http_1_1_output[] =
      "GET / HTTP/1.1\r\n"
      "Host: www.boost.org\r\n"
      "Accept: */*\r\n"
      "Accept-Encoding: identity;q=1.0, *;q=0\r\n"
      "Connection: Close\r\n"
      "\r\n";
  typename http::basic_request<TypeParam>::string_type output_1_0;
  linearize(request, "GET", 1, 0, std::back_inserter(output_1_0));
  EXPECT_EQ(http_1_0_output, output_1_0);
  typename http::basic_request<TypeParam>::string_type output_1_1;
  linearize(request, "GET", 1, 1, std::back_inserter(output_1_1));
  EXPECT_EQ(http_1_1_output, output_1_1);
}

TYPED_TEST(LinearizeTest, OverrideHeaders) {
  http::basic_request<TypeParam> request("http://www.boost.org");
  // We can override the defaulted headers and test that here.
  request << net::header("Accept", "");
  static char http_1_0_no_accept_output[] =
      "GET / HTTP/1.0\r\n"
      "Host: www.boost.org\r\n"
      "Connection: Close\r\n"
      "\r\n";
  static char http_1_1_no_accept_output[] =
      "GET / HTTP/1.1\r\n"
      "Host: www.boost.org\r\n"
      "Accept-Encoding: identity;q=1.0, *;q=0\r\n"
      "Connection: Close\r\n"
      "\r\n";
  typename http::basic_request<TypeParam>::string_type output_1_0;
  linearize(request, "GET", 1, 0, std::back_inserter(output_1_0));
  EXPECT_EQ(http_1_0_no_accept_output, output_1_0);
  typename http::basic_request<TypeParam>::string_type output_1_1;
  linearize(request, "GET", 1, 1, std::back_inserter(output_1_1));
  EXPECT_EQ(http_1_1_no_accept_output, output_1_1);
}
