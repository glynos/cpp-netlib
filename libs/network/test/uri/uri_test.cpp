// Copyright 2009, 2010, 2011 Dean Michael Berris, Jeroen Habraken, Glyn
// Matthews.
// Copyright 2016 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt of copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>
#include <boost/config/warning_disable.hpp>
#include <boost/network/uri.hpp>
#include <boost/network/uri/uri.hpp>
#include <boost/network/uri/uri_io.hpp>
#include <boost/range/algorithm/equal.hpp>
#include <memory>
#include <map>
#include <set>
#include <unordered_set>

using namespace boost::network;

TEST(URITest, basic_uri_scheme_test) {
  uri::uri instance("http://www.example.com/");
  ASSERT_TRUE(uri::valid(instance));
  EXPECT_EQ("http", uri::scheme(instance));
}

TEST(URITest, basic_uri_user_info_test) {
  uri::uri instance("http://www.example.com/");
  ASSERT_TRUE(uri::valid(instance));
  EXPECT_EQ("", uri::user_info(instance));
}

TEST(URITest, basic_uri_host_test) {
  uri::uri instance("http://www.example.com/");
  ASSERT_TRUE(uri::valid(instance));
  EXPECT_EQ("www.example.com", uri::host(instance));
}

TEST(URITest, basic_uri_port_test) {
  uri::uri instance("http://www.example.com/");
  ASSERT_TRUE(uri::valid(instance));
  EXPECT_EQ("", uri::port(instance));
}

TEST(URITest, basic_uri_path_test) {
  uri::uri instance("http://www.example.com/");
  ASSERT_TRUE(uri::valid(instance));
  EXPECT_EQ("/", uri::path(instance));
}

TEST(URITest, basic_uri_query_test) {
  uri::uri instance("http://www.example.com/");
  ASSERT_TRUE(uri::valid(instance));
  EXPECT_EQ("", uri::query(instance));
}

TEST(URITest, basic_uri_fragment_test) {
  uri::uri instance("http://www.example.com/");
  ASSERT_TRUE(uri::valid(instance));
  EXPECT_EQ("", uri::fragment(instance));
}

TEST(URITest, basic_uri_value_semantics_test) {
  uri::uri original;
  uri::uri assigned;
  assigned = original;
  EXPECT_TRUE(original == assigned);
  assigned = "http://www.example.com/";
  EXPECT_TRUE(original != assigned);
  uri::uri copy(assigned);
  EXPECT_TRUE(copy == assigned);
}

TEST(URITest, basic_uri_range_scheme_test) {
  uri::uri instance("http://www.example.com/");
  ASSERT_TRUE(uri::valid(instance));
  EXPECT_TRUE(instance.scheme_range());
  EXPECT_TRUE(instance.begin() == std::begin(instance.scheme_range()));
  EXPECT_TRUE(boost::equal(instance.scheme_range(), boost::as_literal("http")));
}

TEST(URITest, basic_uri_range_user_info_test) {
  uri::uri instance("http://www.example.com/");
  ASSERT_TRUE(uri::valid(instance));
  EXPECT_TRUE(!instance.user_info_range());
  EXPECT_TRUE(std::begin(instance.host_range()) ==
              std::begin(instance.user_info_range()));
  EXPECT_TRUE(std::begin(instance.host_range()) ==
              std::end(instance.user_info_range()));
}

TEST(URITest, basic_uri_range_host_test) {
  uri::uri instance("http://www.example.com/");
  ASSERT_TRUE(uri::valid(instance));
  EXPECT_TRUE(instance.host_range());
  EXPECT_TRUE(boost::equal(instance.host_range(),
                           boost::as_literal("www.example.com")));
}

TEST(URITest, basic_uri_range_port_test) {
  uri::uri instance("http://www.example.com/");
  ASSERT_TRUE(uri::valid(instance));
  EXPECT_TRUE(!instance.port_range());
  EXPECT_TRUE(std::end(instance.host_range()) ==
              std::begin(instance.port_range()));
  EXPECT_TRUE(std::end(instance.host_range()) ==
              std::end(instance.port_range()));
}

TEST(URITest, basic_uri_range_path_test) {
  uri::uri instance("http://www.example.com/");
  ASSERT_TRUE(uri::valid(instance));
  EXPECT_TRUE(instance.path_range());
  EXPECT_TRUE(boost::equal(instance.path_range(), boost::as_literal("/")));
  EXPECT_TRUE(instance.end() == std::end(instance.path_range()));
}

TEST(URITest, basic_uri_range_query_test) {
  uri::uri instance("http://www.example.com/");
  ASSERT_TRUE(uri::valid(instance));
  EXPECT_TRUE(!instance.query_range());
  EXPECT_TRUE(instance.end() == std::begin(instance.query_range()));
  EXPECT_TRUE(instance.end() == std::end(instance.query_range()));
}

TEST(URITest, basic_uri_range_fragment_test) {
  uri::uri instance("http://www.example.com/");
  ASSERT_TRUE(uri::valid(instance));
  EXPECT_TRUE(!instance.fragment_range());
  EXPECT_TRUE(instance.end() == std::begin(instance.fragment_range()));
  EXPECT_TRUE(instance.end() == std::end(instance.fragment_range()));
}

TEST(URITest, full_uri_scheme_test) {
  uri::uri instance(
      "http://user:password@www.example.com:80/path?query#fragment");
  ASSERT_TRUE(uri::valid(instance));
  EXPECT_EQ("http", uri::scheme(instance));
}

TEST(URITest, full_uri_user_info_test) {
  uri::uri instance(
      "http://user:password@www.example.com:80/path?query#fragment");
  ASSERT_TRUE(uri::valid(instance));
  EXPECT_EQ("user:password", uri::user_info(instance));
}

TEST(URITest, full_uri_host_test) {
  uri::uri instance(
      "http://user:password@www.example.com:80/path?query#fragment");
  ASSERT_TRUE(uri::valid(instance));
  EXPECT_EQ("www.example.com", uri::host(instance));
}

TEST(URITest, full_uri_port_test) {
  uri::uri instance(
      "http://user:password@www.example.com:80/path?query#fragment");
  ASSERT_TRUE(uri::valid(instance));
  EXPECT_EQ("80", uri::port(instance));
  EXPECT_TRUE(uri::port_us(instance));
  EXPECT_EQ(80, uri::port_us(instance).get());
}

TEST(URITest, full_uri_path_test) {
  uri::uri instance(
      "http://user:password@www.example.com:80/path?query#fragment");
  ASSERT_TRUE(uri::valid(instance));
  EXPECT_EQ("/path", uri::path(instance));
}

TEST(URITest, full_uri_query_test) {
  uri::uri instance(
      "http://user:password@www.example.com:80/path?query#fragment");
  ASSERT_TRUE(uri::valid(instance));
  EXPECT_EQ("query", uri::query(instance));
}

TEST(URITest, full_uri_fragment_test) {
  uri::uri instance(
      "http://user:password@www.example.com:80/path?query#fragment");
  ASSERT_TRUE(uri::valid(instance));
  EXPECT_EQ("fragment", uri::fragment(instance));
}

TEST(URITest, full_uri_range_scheme_test) {
  uri::uri instance(
      "http://user:password@www.example.com:80/path?query#fragment");
  ASSERT_TRUE(uri::valid(instance));
  EXPECT_TRUE(instance.scheme_range());
  EXPECT_TRUE(instance.begin() == std::begin(instance.scheme_range()));
  EXPECT_TRUE(boost::equal(instance.scheme_range(), boost::as_literal("http")));
}

TEST(URITest, full_uri_range_user_info_test) {
  uri::uri instance(
      "http://user:password@www.example.com:80/path?query#fragment");
  ASSERT_TRUE(uri::valid(instance));
  EXPECT_TRUE(instance.user_info_range());
  EXPECT_TRUE(boost::equal(instance.user_info_range(),
                           boost::as_literal("user:password")));
}

TEST(URITest, full_uri_range_host_test) {
  uri::uri instance(
      "http://user:password@www.example.com:80/path?query#fragment");
  ASSERT_TRUE(uri::valid(instance));
  EXPECT_TRUE(instance.host_range());
  EXPECT_TRUE(boost::equal(instance.host_range(),
                           boost::as_literal("www.example.com")));
}

TEST(URITest, full_uri_range_port_test) {
  uri::uri instance(
      "http://user:password@www.example.com:80/path?query#fragment");
  ASSERT_TRUE(uri::valid(instance));
  EXPECT_TRUE(instance.port_range());
  EXPECT_TRUE(boost::equal(instance.port_range(), boost::as_literal("80")));
}

TEST(URITest, full_uri_range_path_test) {
  uri::uri instance(
      "http://user:password@www.example.com:80/path?query#fragment");
  ASSERT_TRUE(uri::valid(instance));
  EXPECT_TRUE(instance.path_range());
  EXPECT_TRUE(boost::equal(instance.path_range(), boost::as_literal("/path")));
}

TEST(URITest, full_uri_range_query_test) {
  uri::uri instance(
      "http://user:password@www.example.com:80/path?query#fragment");
  ASSERT_TRUE(uri::valid(instance));
  EXPECT_TRUE(instance.query_range());
  EXPECT_TRUE(boost::equal(instance.query_range(), boost::as_literal("query")));
}

TEST(URITest, full_uri_range_fragment_test) {
  uri::uri instance(
      "http://user:password@www.example.com:80/path?query#fragment");
  ASSERT_TRUE(uri::valid(instance));
  EXPECT_TRUE(instance.fragment_range());
  EXPECT_TRUE(
      boost::equal(instance.fragment_range(), boost::as_literal("fragment")));
  EXPECT_TRUE(instance.end() == std::end(instance.fragment_range()));
}

TEST(URITest, mailto_test) {
  uri::uri instance("mailto:john.doe@example.com");
  ASSERT_TRUE(uri::valid(instance));
  EXPECT_EQ("mailto", uri::scheme(instance));
  EXPECT_EQ("john.doe@example.com", uri::path(instance));
}

TEST(URITest, file_test) {
  uri::uri instance("file:///bin/bash");
  ASSERT_TRUE(uri::valid(instance));
  EXPECT_EQ("file", uri::scheme(instance));
  EXPECT_EQ("/bin/bash", uri::path(instance));
}

TEST(URITest, xmpp_test) {
  uri::uri instance(
      "xmpp:example-node@example.com?message;subject=Hello%20World");
  ASSERT_TRUE(uri::valid(instance));
  EXPECT_EQ("xmpp", uri::scheme(instance));
  EXPECT_EQ("example-node@example.com", uri::path(instance));
  EXPECT_EQ("message;subject=Hello%20World", uri::query(instance));
}

TEST(URITest, ipv4_address_test) {
  uri::uri instance("http://129.79.245.252/");
  ASSERT_TRUE(uri::valid(instance));
  EXPECT_EQ("http", uri::scheme(instance));
  EXPECT_EQ("129.79.245.252", uri::host(instance));
  EXPECT_EQ("/", uri::path(instance));
}

TEST(URITest, ipv4_loopback_test) {
  uri::uri instance("http://127.0.0.1/");
  ASSERT_TRUE(uri::valid(instance));
  EXPECT_EQ("http", uri::scheme(instance));
  EXPECT_EQ("127.0.0.1", uri::host(instance));
  EXPECT_EQ("/", uri::path(instance));
}

TEST(URITest, ipv6_address_test_1) {
  uri::uri instance("http://[1080:0:0:0:8:800:200C:417A]/");
  ASSERT_TRUE(uri::valid(instance));
  EXPECT_EQ("http", uri::scheme(instance));
  EXPECT_EQ("[1080:0:0:0:8:800:200C:417A]", uri::host(instance));
  EXPECT_EQ("/", uri::path(instance));
}

TEST(URITest, ipv6_address_test_2) {
  uri::uri instance("http://[2001:db8:85a3:8d3:1319:8a2e:370:7348]/");
  ASSERT_TRUE(uri::valid(instance));
  EXPECT_EQ("http", uri::scheme(instance));
  EXPECT_EQ("[2001:db8:85a3:8d3:1319:8a2e:370:7348]", uri::host(instance));
  EXPECT_EQ("/", uri::path(instance));
}

// TEST(URITest, ipv6_loopback_test) {
//    uri::uri instance("http://[::1]/");
//    ASSERT_TRUE(uri::valid(instance));
//    EXPECT_EQ("http", uri::scheme(instance));
//    EXPECT_EQ("[::1]", uri::host(instance));
//    EXPECT_EQ("/", uri::path(instance));
//}

TEST(URITest, ftp_test) {
  uri::uri instance("ftp://john.doe@ftp.example.com/");
  ASSERT_TRUE(uri::valid(instance));
  EXPECT_EQ("ftp", uri::scheme(instance));
  EXPECT_EQ("john.doe", uri::user_info(instance));
  EXPECT_EQ("ftp.example.com", uri::host(instance));
  EXPECT_EQ("/", uri::path(instance));
}

TEST(URITest, news_test) {
  uri::uri instance("news:comp.infosystems.www.servers.unix");
  ASSERT_TRUE(uri::valid(instance));
  EXPECT_EQ("news", uri::scheme(instance));
  EXPECT_EQ("comp.infosystems.www.servers.unix", uri::path(instance));
}

TEST(URITest, tel_test) {
  uri::uri instance("tel:+1-816-555-1212");
  ASSERT_TRUE(uri::valid(instance));
  EXPECT_EQ("tel", uri::scheme(instance));
  EXPECT_EQ("+1-816-555-1212", uri::path(instance));
}

TEST(URITest, encoded_uri_test) {
  uri::uri instance(
      "http://www.example.com/"
      "Path%20With%20%28Some%29%20Encoded%20Characters%21");
  ASSERT_TRUE(uri::valid(instance));
  EXPECT_EQ("http", uri::scheme(instance));
  EXPECT_EQ("www.example.com", uri::host(instance));
  EXPECT_EQ("/Path%20With%20%28Some%29%20Encoded%20Characters%21", uri::path(instance));
  EXPECT_EQ("/Path With (Some) Encoded Characters!",uri::decoded_path(instance));
}

TEST(URITest, copy_constructor_test) {
  uri::uri instance("http://www.example.com/");
  uri::uri copy = instance;
  EXPECT_EQ(instance, copy);
}

TEST(URITest, assignment_test) {
  uri::uri instance("http://www.example.com/");
  uri::uri copy;
  copy = instance;
  EXPECT_EQ(instance, copy);
}

TEST(URITest, swap_test) {
  uri::uri instance("http://www.example.com/");
  uri::uri copy("http://www.example.org/");
  uri::swap(instance, copy);
  EXPECT_EQ("http://www.example.org/", instance.string());
  EXPECT_EQ("http://www.example.com/", copy.string());
}

TEST(URITest, equality_test) {
  uri::uri uri_1("http://www.example.com/");
  uri::uri uri_2("http://www.example.com/");
  EXPECT_TRUE(uri_1 == uri_2);
}

TEST(URITest, equality_test_1) {
  uri::uri uri_1("http://www.example.com/");
  std::string uri_2("http://www.example.com/");
  EXPECT_TRUE(uri_1 == uri_2);
}

TEST(URITest, equality_test_2) {
  std::string uri_1("http://www.example.com/");
  uri::uri uri_2("http://www.example.com/");
  EXPECT_TRUE(uri_1 == uri_2);
}

TEST(URITest, equality_test_3) {
  uri::uri uri_1("http://www.example.com/");
  std::string uri_2("http://www.example.com/");
  EXPECT_TRUE(uri_1 == uri_2.c_str());
}

TEST(URITest, equality_test_4) {
  std::string uri_1("http://www.example.com/");
  uri::uri uri_2("http://www.example.com/");
  EXPECT_TRUE(uri_1.c_str() == uri_2);
}

TEST(URITest, inequality_test) {
  uri::uri uri_1("http://www.example.com/");
  uri::uri uri_2("http://www.example.com/");
  EXPECT_TRUE(!(uri_1 != uri_2));
}

TEST(URITest, less_than_test) {
  // uri_1 is lexicographically less than uri_2
  uri::uri uri_1("http://www.example.com/");
  uri::uri uri_2("http://www.example.org/");
  EXPECT_TRUE(uri_1 < uri_2);
}

TEST(URITest, username_test) {
  uri::uri instance("ftp://john.doe@ftp.example.com/");
  ASSERT_TRUE(uri::valid(instance));
  EXPECT_EQ("john.doe", uri::username(instance));
}

TEST(URITest, pasword_test) {
  uri::uri instance("ftp://john.doe:password@ftp.example.com/");
  ASSERT_TRUE(uri::valid(instance));
  EXPECT_EQ("password", uri::password(instance));
}

TEST(URITest, hierarchical_part_test) {
  uri::uri instance(
      "http://user:password@www.example.com:80/path?query#fragment");
  ASSERT_TRUE(uri::valid(instance));
  EXPECT_EQ("user:password@www.example.com:80/path", uri::hierarchical_part(instance));
}

TEST(URITest, partial_hierarchical_part_test) {
  uri::uri instance("http://www.example.com?query#fragment");
  ASSERT_TRUE(uri::valid(instance));
  EXPECT_EQ("www.example.com", uri::hierarchical_part(instance));
}

TEST(URITest, authority_test) {
  uri::uri instance(
      "http://user:password@www.example.com:80/path?query#fragment");
  ASSERT_TRUE(uri::valid(instance));
  EXPECT_EQ("user:password@www.example.com:80", uri::authority(instance));
}

TEST(URITest, partial_authority_test) {
  uri::uri instance("http://www.example.com/path?query#fragment");
  ASSERT_TRUE(uri::valid(instance));
  EXPECT_EQ("www.example.com", uri::authority(instance));
}

TEST(URITest, http_query_map_test) {
  uri::uri instance(
      "http://user:password@www.example.com:80/path?query=something#fragment");
  ASSERT_TRUE(uri::valid(instance));

  std::map<std::string, std::string> queries;
  uri::query_map(instance, queries);
  ASSERT_EQ(queries.size(), std::size_t(1));
  EXPECT_EQ("query", queries.begin()->first);
  EXPECT_EQ("something", queries.begin()->second);
}

TEST(URITest, xmpp_query_map_test) {
  uri::uri instance(
      "xmpp:example-node@example.com?message;subject=Hello%20World");
  ASSERT_TRUE(uri::valid(instance));

  std::map<std::string, std::string> queries;
  uri::query_map(instance, queries);
  ASSERT_EQ(queries.size(), std::size_t(2));
  EXPECT_EQ("message", queries.begin()->first);
  EXPECT_EQ("", queries.begin()->second);
  EXPECT_EQ("subject", (++queries.begin())->first);
  EXPECT_EQ("Hello%20World", (++queries.begin())->second);
}

TEST(URITest, range_test) {
  const std::string url("http://www.example.com/");
  uri::uri instance(url);
  ASSERT_TRUE(uri::valid(instance));
  EXPECT_TRUE(boost::equal(instance, url));
}

TEST(URITest, issue_67_test) {
  // https://github.com/cpp-netlib/cpp-netlib/issues/67
  const std::string site_name("http://www.google.com");
  uri::uri bar0;
  uri::uri bar1 = site_name;
  bar0 = site_name;
  EXPECT_TRUE(uri::is_valid(bar0));
  EXPECT_TRUE(uri::is_valid(bar1));
}

TEST(URITest, from_parts_1) {
  EXPECT_EQ(uri::uri("http://www.example.com/path?query#fragment"),
                    uri::from_parts(uri::uri("http://www.example.com"), "/path",
                                    "query", "fragment"));
}

TEST(URITest, from_parts_2) {
  EXPECT_EQ(
      uri::uri("http://www.example.com/path?query#fragment"),
      uri::from_parts("http://www.example.com", "/path", "query", "fragment"));
}

TEST(URITest, from_parts_3) {
  EXPECT_EQ(
      uri::uri("http://www.example.com/path?query"),
      uri::from_parts("http://www.example.com", "/path", "query"));
}

TEST(URITest, from_parts_4) {
  EXPECT_EQ(uri::uri("http://www.example.com/path"),
                    uri::from_parts("http://www.example.com", "/path"));
}

TEST(URITest, issue_104_test) {
  // https://github.com/cpp-netlib/cpp-netlib/issues/104
  std::unique_ptr<uri::uri> instance(new uri::uri("http://www.example.com/"));
  uri::uri copy = *instance;
  instance.reset();
  EXPECT_EQ("http", uri::scheme(copy));
}

TEST(URITest, uri_set_test) {
  std::set<uri::uri> uri_set;
  uri_set.insert(uri::uri("http://www.example.com/"));
  ASSERT_TRUE(!uri_set.empty());
  EXPECT_EQ(uri::uri("http://www.example.com/"), (*uri_set.begin()));
}

TEST(URITest, uri_unordered_set_test) {
  std::unordered_set<uri::uri> uri_set;
  uri_set.insert(uri::uri("http://www.example.com/"));
  ASSERT_TRUE(!uri_set.empty());
  EXPECT_EQ(uri::uri("http://www.example.com/"), (*uri_set.begin()));
}

TEST(URITest, issue_161_test) {
  uri::uri instance(
      "http://www.example.com/"
      "path?param1=-&param2=some+plus+encoded+text&param3=~");
  ASSERT_TRUE(uri::valid(instance));

  std::map<std::string, std::string> queries;
  uri::query_map(instance, queries);
  ASSERT_EQ(3, queries.size());
  EXPECT_EQ("-", queries["param1"]);
  EXPECT_EQ("some+plus+encoded+text", queries["param2"]);
  EXPECT_EQ("~", queries["param3"]);
  EXPECT_EQ("some plus encoded text", uri::decoded(queries["param2"]));
}

TEST(URITest, issue_364_test) {
  uri::uri instance;
  uri::schemes::http(instance) << uri::host("my.awesome.server.com");
  EXPECT_EQ("my.awesome.server.com", uri::authority(instance));
}

TEST(URITest, issue_447_test) {
  uri::uri instance("http://[www.foo.com/");
  ASSERT_TRUE(!uri::valid(instance));
}

TEST(URITest, issue_499_test) {
  uri::uri instance(
      "http://www.example.com/path?param1&param2=&param3=value");
  ASSERT_TRUE(uri::valid(instance));

  std::map<std::string, std::string> queries;
  uri::query_map(instance, queries);
  ASSERT_EQ(3, queries.size());
  EXPECT_EQ("", queries["param1"]);
  EXPECT_EQ("", queries["param2"]);
  EXPECT_EQ("value", queries["param3"]);
}
