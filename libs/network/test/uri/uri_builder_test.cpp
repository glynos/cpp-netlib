// Copyright (c) Glyn Matthews 2012.
// Copyright 2016 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>
#include <boost/config/warning_disable.hpp>
#include <boost/network/uri/uri.hpp>
#include <boost/network/uri/uri_io.hpp>

using namespace boost::network;

TEST(BuilderTest, Simple) {
  uri::uri instance;
  uri::builder builder(instance);
  builder.scheme("http").host("www.example.com").path("/");
  ASSERT_TRUE(uri::valid(instance));
  EXPECT_EQ("http://www.example.com/", instance.string());
}

TEST(BuilderTest, Full) {
  uri::uri instance;
  uri::builder builder(instance);
  builder.scheme("http")
      .user_info("user:password")
      .host("www.example.com")
      .port("80")
      .path("/path")
      .query("query")
      .fragment("fragment");
  ASSERT_TRUE(uri::valid(instance));
  EXPECT_EQ(
      "http://user:password@www.example.com:80/path?query#fragment",
      instance.string());
}

TEST(BuilderTest, Port) {
  uri::uri instance;
  uri::builder(instance).scheme("http").host("www.example.com").port(8000).path(
      "/");
  ASSERT_TRUE(uri::valid(instance));
  EXPECT_EQ("http://www.example.com:8000/", instance.string());
}

TEST(BuilderTest, EncodedPath) {
  uri::uri instance;
  uri::builder builder(instance);
  builder.scheme("http").host("www.example.com").port(8000).encoded_path(
      "/Path With (Some) Encoded Characters!");
  ASSERT_TRUE(uri::valid(instance));
  EXPECT_EQ(
      "http://www.example.com:8000/"
      "Path%20With%20%28Some%29%20Encoded%20Characters%21",
      instance.string());
}

TEST(BuilderTest, Query) {
  uri::uri instance;
  uri::builder builder(instance);
  builder.scheme("http").host("www.example.com").path("/").query("key",
                                                                 "value");
  ASSERT_TRUE(uri::valid(instance));
  EXPECT_EQ("http://www.example.com/?key=value", instance.string());
}

TEST(BuilderTest, TwoQueries) {
  uri::uri instance;
  uri::builder builder(instance);
  builder.scheme("http")
      .host("www.example.com")
      .path("/")
      .query("key1", "value1")
      .query("key2", "value2");
  ASSERT_TRUE(uri::valid(instance));
  EXPECT_EQ("http://www.example.com/?key1=value1&key2=value2",
                    instance.string());
}

TEST(BuilderTest, Fragment) {
  uri::uri instance;
  uri::builder builder(instance);
  builder.scheme("http").host("www.example.com").path("/").fragment("fragment");
  ASSERT_TRUE(uri::valid(instance));
  EXPECT_EQ("http://www.example.com/#fragment", instance.string());
}

TEST(BuilderTest, FromBase) {
  uri::uri instance("http://www.example.com");
  uri::builder builder(instance);
  builder.path("/").fragment("fragment");
  ASSERT_TRUE(uri::valid(instance));
  EXPECT_EQ("http://www.example.com/#fragment", instance.string());
}

TEST(BuilderTest, EncodedNull) {
  // there is a potential bug in the way we process ranges if the
  // strings are null terminated.
  uri::uri instance;
  uri::builder builder(instance);
  builder.scheme("http").host("www.example.com").encoded_path("/");
  ASSERT_TRUE(uri::valid(instance));
  EXPECT_EQ("http://www.example.com/", instance.string());
}

TEST(BuilderTest, MailtoScheme) {
  uri::uri instance;
  uri::builder builder(instance);
  builder.scheme("mailto").path("cpp-netlib@example.com");
  ASSERT_TRUE(uri::valid(instance));
  EXPECT_EQ("mailto:cpp-netlib@example.com", instance.string());
}

TEST(BuilderTest, IPv4) {
  using namespace boost::asio::ip;
  uri::uri instance;
  uri::builder builder(instance);
  builder.scheme("http").host(address_v4::loopback()).path("/");
  ASSERT_TRUE(uri::valid(instance));
  EXPECT_EQ("http://127.0.0.1/", instance.string());
}

// TEST(BuilderTest, IPv6) {
//    using namespace asio::ip;
//    uri::uri instance;
//    uri::builder builder(instance);
//    builder
//        .scheme("http")
//        .host(address_v6::loopback())
//        .path("/")
//        ;
//    ASSERT_TRUE(uri::valid(instance));
//    EXPECT_EQ("http://[::1]/", instance.string());
//}
