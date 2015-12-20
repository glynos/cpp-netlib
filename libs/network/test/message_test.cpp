
// Copyright Dean Michael Berris 2007.
// Copyright 2015 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// Migrated from using Boost.Test to using googletest instead.
#include <gtest/gtest.h>
#include <algorithm>
#include <boost/network/include/message.hpp>
#include <boost/network/message/directives.hpp>
#include <boost/network/protocol/http/tags.hpp>

namespace {

namespace http = boost::network::http;
namespace network = boost::network;

template <class T>
class MessageTest : public ::testing::Test {
};

using TagTypes = ::testing::Types<http::tags::http_default_8bit_tcp_resolve,
                                  http::tags::http_default_8bit_udp_resolve,
                                  network::tags::default_string>;
TYPED_TEST_CASE(MessageTest, TagTypes);

TYPED_TEST(MessageTest, Constructors){
  network::basic_message<TypeParam> message;  // default construction
  auto copy = message;  // copy construction
  ASSERT_TRUE(copy == message);
}

TYPED_TEST(MessageTest, Equality) {
  // Create an original message.
  network::basic_message<TypeParam> message;
  message << network::source("source") << network::destination("destination")
          << network::header("key", "value") << network::body("body");

  // Create the identical message.
  network::basic_message<TypeParam> other;
  other << network::source("source") << network::destination("destination")
        << network::header("key", "value") << network::body("body");

  ASSERT_TRUE(message == other);
}

TYPED_TEST(MessageTest, Inequality) {
  // Create an original message.
  network::basic_message<TypeParam> message;
  message << network::source("source") << network::destination("destination")
          << network::header("key", "value") << network::body("body");

  // Create a different message.
  network::basic_message<TypeParam> other;
  other << network::source("source") << network::destination("destination")
        << network::header("key", "value") << network::body("different body!");

  ASSERT_FALSE(message == other);
}

}  // namespace
