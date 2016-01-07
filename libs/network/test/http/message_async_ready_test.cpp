
// Copyright 2010 Dean Michael Berris.
// Copyright 2016 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>
#include <boost/network/include/http/client.hpp>
#include "tag_types.hpp"

namespace http = boost::network::http;

template <class T>
class MessageTest : public ::testing::Test {};

TYPED_TEST_CASE(MessageTest, TagTypes);

TYPED_TEST(MessageTest, UnreadyStateResponseTest) {
  using response = http::basic_response<TypeParam>;
  response r;
  EXPECT_FALSE(ready(r));
}
