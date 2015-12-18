
// Copyright Dean Michael Berris 2007.
// Copyright 2015, Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>
#include <algorithm>
#include <boost/network/include/message.hpp>

TEST(MessageTransformTest, TransformToUpper) {
  using namespace boost::network;

  message msg;
  msg << source("me");
  ASSERT_EQ("me",source(msg));
  msg << transform(to_upper_, source_);
  ASSERT_EQ("ME",source(msg));
  msg << destination("you");
  ASSERT_EQ("you",destination(msg));
  msg << transform(to_upper_, destination_);
  ASSERT_EQ("YOU",destination(msg));
}

TEST(MessageTransformTest, TransformToLower) {
  using namespace boost::network;

  message msg;
  msg << source("ME");
  ASSERT_EQ("ME",source(msg));
  msg << transform(to_lower_, source_);
  ASSERT_EQ("me",source(msg));
  msg << destination("YOU");
  ASSERT_EQ("YOU",destination(msg));
  msg << transform(to_lower_, destination_);
  ASSERT_EQ("you",destination(msg));
}
