// Copyright (c) Glyn Matthews 2011.
// Copyright 2016 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>
#include <boost/config/warning_disable.hpp>
#include <boost/network/uri/encode.hpp>
#include <boost/network/uri/decode.hpp>
#include <iterator>

using namespace boost::network;

TEST(URIEncodingTest, Encoding) {
  const std::string unencoded(" !\"#$%&\'()*");
  const std::string encoded("%20%21%22%23%24%25%26%27%28%29%2A");

  std::string instance;
  uri::encode(unencoded, std::back_inserter(instance));
  EXPECT_EQ(instance, encoded);
}

TEST(URIEncodingTest, Decoding) {
  const std::string unencoded(" !\"#$%&\'()*");
  const std::string encoded("%20%21%22%23%24%25%26%27%28%29%2A");

  std::string instance;
  uri::decode(encoded, std::back_inserter(instance));
  EXPECT_EQ(instance, unencoded);
}

TEST(URIEncodingTest, EncodingMultibyte) {
  const std::string unencoded("한글 테스트");
  const std::string encoded("%ED%95%9C%EA%B8%80%20%ED%85%8C%EC%8A%A4%ED%8A%B8");

  std::string instance;
  uri::encode(unencoded, std::back_inserter(instance));
  EXPECT_EQ(instance, encoded);
}

TEST(URIEncodingTest, DecodingMultibyte) {
  const std::string unencoded("한글 테스트");
  const std::string encoded("%ED%95%9C%EA%B8%80%20%ED%85%8C%EC%8A%A4%ED%8A%B8");

  std::string instance;
  uri::decode(encoded, std::back_inserter(instance));
  EXPECT_EQ(instance, unencoded);
}

TEST(URIEncodingTest, DecodingThrows) {
  const std::string encoded("%");

  std::string instance;
  EXPECT_THROW(uri::decoded(encoded), std::out_of_range);
}
