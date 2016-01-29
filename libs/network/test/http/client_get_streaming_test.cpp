// Copyright 2011 Dean Michael Berris &lt;mikhailberis@gmail.com&gt;.
// Copyright 2016 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>
#include <boost/network/include/http/client.hpp>
#include <iostream>
#include "client_types.hpp"

namespace net = boost::network;
namespace http = boost::network::http;

struct body_handler {
  explicit body_handler(std::string& body) : body(body) {}

  BOOST_NETWORK_HTTP_BODY_CALLBACK(operator(), range, error) {
    (void)error;
    body.append(std::begin(range), std::end(range));
  }

  std::string& body;
};

TYPED_TEST_CASE(HTTPClientTest, ClientTypes);

TYPED_TEST(HTTPClientTest, GetStreamingTest) {
  typename TypeParam::request request("http://www.boost.org");
  typename TypeParam::response response;
  typename TypeParam::string_type body_string;
  typename TypeParam::string_type dummy_body;
  body_handler handler_instance(body_string);
  {
    TypeParam client_;
    ASSERT_NO_THROW(response = client_.get(request, handler_instance));
    auto range = headers(response)["Content-Type"];
    ASSERT_TRUE(!boost::empty(range));
    EXPECT_EQ(0u, body(response).size());
    EXPECT_EQ("HTTP/1.", response.version().substr(0, 7));
    EXPECT_EQ(200u, response.status());
    EXPECT_EQ("OK", response.status_message());
    dummy_body = body(response);
  }
  EXPECT_EQ(dummy_body, typename TypeParam::string_type());
}
