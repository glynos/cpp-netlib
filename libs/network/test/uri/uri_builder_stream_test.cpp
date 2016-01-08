// Copyright (c) Glyn Matthews 2011.
// Copyright 2016 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>
#include <boost/config/warning_disable.hpp>
#include <boost/network/uri/uri.hpp>
#include <boost/network/uri/directives.hpp>
#include <boost/network/uri/uri_io.hpp>

using namespace boost::network;

TEST(BuilderTest, Simple) {
  uri::uri instance;
  instance << uri::scheme("http") << uri::host("www.example.com")
           << uri::path("/");
  ASSERT_TRUE(uri::valid(instance));
  EXPECT_EQ("http://www.example.com/", instance.string());
}

TEST(BuilderTest, Full) {
  uri::uri instance;
  instance << uri::scheme("http") << uri::user_info("user:password")
           << uri::host("www.example.com") << uri::port("80")
           << uri::path("/path") << uri::query("query")
           << uri::fragment("fragment");
  ASSERT_TRUE(uri::valid(instance));
  EXPECT_EQ(
      "http://user:password@www.example.com:80/path?query#fragment",
      instance.string());
}

TEST(BuilderTest, Port) {
  uri::uri instance;
  instance << uri::scheme("http") << uri::host("www.example.com")
           << uri::port(8000) << uri::path("/");
  ASSERT_TRUE(uri::valid(instance));
  EXPECT_EQ("http://www.example.com:8000/", instance.string());
}

TEST(BuilderTest, EncodedPath) {
  uri::uri instance;
  instance << uri::scheme("http") << uri::host("www.example.com")
           << uri::port(8000)
           << uri::encoded_path("/Path With (Some) Encoded Characters!");
  ;
  ASSERT_TRUE(uri::valid(instance));
  EXPECT_EQ(
      "http://www.example.com:8000/"
      "Path%20With%20%28Some%29%20Encoded%20Characters%21",
      instance.string());
}

TEST(BuilderTest, Query) {
  uri::uri instance;
  instance << uri::scheme("http") << uri::host("www.example.com")
           << uri::path("/") << uri::query("key", "value");
  ASSERT_TRUE(uri::valid(instance));
  EXPECT_EQ("http://www.example.com/?key=value", instance.string());
}

TEST(BuilderTest, TwoQueries) {
  uri::uri instance;
  instance << uri::scheme("http") << uri::host("www.example.com")
           << uri::path("/") << uri::query("key1", "value1")
           << uri::query("key2", "value2");
  ASSERT_TRUE(uri::valid(instance));
  EXPECT_EQ("http://www.example.com/?key1=value1&key2=value2",
                    instance.string());
}

TEST(BuilderTest, Fragment) {
  uri::uri instance;
  instance << uri::scheme("http") << uri::host("www.example.com")
           << uri::path("/") << uri::fragment("fragment");
  ASSERT_TRUE(uri::valid(instance));
  EXPECT_EQ("http://www.example.com/#fragment", instance.string());
}

TEST(BuilderTest, FromBase) {
  uri::uri base_uri("http://www.example.com");
  uri::uri instance;
  instance << base_uri << uri::path("/") << uri::fragment("fragment");
  ASSERT_TRUE(uri::valid(instance));
  EXPECT_EQ("http://www.example.com/#fragment", instance.string());
}

TEST(BuilderTest, Scheme) {
  uri::uri instance;
  instance << uri::schemes::http << uri::host("www.example.com")
           << uri::path("/");
  ASSERT_TRUE(uri::valid(instance));
  EXPECT_EQ("http://www.example.com/", instance.string());
}

TEST(BuilderTest, HTTPSScheme) {
  uri::uri instance;
  instance << uri::schemes::https << uri::host("www.example.com")
           << uri::path("/");
  ASSERT_TRUE(uri::valid(instance));
  EXPECT_EQ("https://www.example.com/", instance.string());
}

TEST(BuilderTest, EncodedNull) {
  // there is a potential bug in the way we process ranges if the
  // strings are null terminated.
  uri::uri instance;
  instance << uri::scheme("http") << uri::host("www.example.com")
           << uri::encoded_path("/");
  ASSERT_TRUE(uri::valid(instance));
  EXPECT_EQ("http://www.example.com/", instance.string());
}

TEST(BuilderTest, MailtoScheme) {
  uri::uri instance;
  instance << uri::scheme("mailto") << uri::path("cpp-netlib@example.com");
  ASSERT_TRUE(uri::valid(instance));
  EXPECT_EQ("mailto:cpp-netlib@example.com", instance.string());
}
