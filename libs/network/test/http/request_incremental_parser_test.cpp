// Copyright 2010 Dean Michael Berris.
// Copyright 2016 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>
#include <boost/config/warning_disable.hpp>
#include <boost/network/protocol/http/server/request_parser.hpp>
#include <boost/network/tags.hpp>
#include <boost/range.hpp>
#include <boost/logic/tribool.hpp>
#include <string>
#include <iostream>

/** Synopsis
 *
 *  Test for the HTTP Request Incremental Parser
 *  --------------------------------------------
 *
 *  In this test we fully intend to specify how an incremental HTTP request
 *  parser should be used. This follows the HTTP Response Incremental Parser
 *  example, and models the Incremental Parser Concept.
 *
 */

namespace tags = boost::network::tags;
namespace logic = boost::logic;
using namespace boost::network::http;

TEST(IncrementalRequestParserTest, Constructor) {
  request_parser<tags::default_string> p;  // default constructible
}

TEST(IncrementalRequestParserTest, ParseMethod) {
  request_parser<tags::default_string> p;
  logic::tribool parsed_ok = false;
  typedef request_parser<tags::default_string> request_parser_type;
  typedef boost::iterator_range<std::string::const_iterator> range_type;
  range_type result_range;

  std::string valid_http_method = "GET ";
  std::tie(parsed_ok, result_range) =
      p.parse_until(request_parser_type::method_done, valid_http_method);
  EXPECT_TRUE(parsed_ok);
  EXPECT_FALSE(boost::empty(result_range));
  std::string parsed(std::begin(result_range), std::end(result_range));
  std::cout << "PARSED: " << parsed << " [state:" << p.state() << "] "
            << std::endl;

  std::string invalid_http_method = "get ";
  p.reset();
  std::tie(parsed_ok, result_range) =
      p.parse_until(request_parser_type::method_done, invalid_http_method);
  EXPECT_EQ(false, parsed_ok);
  parsed.assign(std::begin(result_range), std::end(result_range));
  std::cout << "PARSED: " << parsed << " [state:" << p.state() << "] "
            << std::endl;
}

TEST(IncrementalRequestParserTest, ParseURI) {
  request_parser<tags::default_string> p;
  logic::tribool parsed_ok = false;
  typedef request_parser<tags::default_string> request_parser_type;
  typedef boost::iterator_range<std::string::const_iterator> range_type;
  range_type result_range;

  std::string valid_http_request = "GET / HTTP/1.1\r\n";
  std::tie(parsed_ok, result_range) =
      p.parse_until(request_parser_type::uri_done, valid_http_request);
  EXPECT_EQ(true, parsed_ok);
  EXPECT_FALSE(boost::empty(result_range));
  std::string parsed(std::begin(result_range), std::end(result_range));
  std::cout << "PARSED: " << parsed << " [state:" << p.state() << "] "
            << std::endl;

  std::string invalid_http_request = "GET /\t HTTP/1.1\r\n";
  p.reset();
  std::tie(parsed_ok, result_range) =
      p.parse_until(request_parser_type::uri_done, invalid_http_request);
  EXPECT_EQ(false, parsed_ok);
  parsed.assign(std::begin(result_range), std::end(result_range));
  std::cout << "PARSED: " << parsed << " [state:" << p.state() << "] "
            << std::endl;
}

TEST(IncrementalRequestParserTest, ParseHTTPVersion) {
  request_parser<tags::default_string> p;
  logic::tribool parsed_ok = false;
  typedef request_parser<tags::default_string> request_parser_type;
  typedef boost::iterator_range<std::string::const_iterator> range_type;
  range_type result_range;

  std::string valid_http_request = "GET / HTTP/1.1\r\n";
  std::tie(parsed_ok, result_range) =
      p.parse_until(request_parser_type::version_done, valid_http_request);
  EXPECT_EQ(true, parsed_ok);
  EXPECT_FALSE(boost::empty(result_range));
  std::string parsed(std::begin(result_range), std::end(result_range));
  std::cout << "PARSED: " << parsed << " [state:" << p.state() << "] "
            << std::endl;

  std::string invalid_http_request = "GET / HTTP 1.1\r\n";
  p.reset();
  std::tie(parsed_ok, result_range) =
      p.parse_until(request_parser_type::version_done, invalid_http_request);
  EXPECT_EQ(false, parsed_ok);
  parsed.assign(std::begin(result_range), std::end(result_range));
  std::cout << "PARSED: " << parsed << " [state:" << p.state() << "] "
            << std::endl;
}

TEST(IncrementalRequestParserTest, ParseHTTPHeaders) {
  request_parser<tags::default_string> p;
  logic::tribool parsed_ok = false;
  typedef request_parser<tags::default_string> request_parser_type;
  typedef boost::iterator_range<std::string::const_iterator> range_type;
  range_type result_range;

  std::string valid_http_request =
      "GET / HTTP/1.1\r\nHost: cpp-netlib.org\r\n\r\n";
  std::tie(parsed_ok, result_range) =
      p.parse_until(request_parser_type::headers_done, valid_http_request);
  EXPECT_EQ(true, parsed_ok);
  EXPECT_FALSE(boost::empty(result_range));
  std::string parsed(std::begin(result_range), std::end(result_range));
  std::cout << "PARSED: " << parsed << " [state:" << p.state() << "] "
            << std::endl;

  valid_http_request =
      "GET / HTTP/1.1\r\nHost: cpp-netlib.org\r\nConnection: close\r\n\r\n";
  p.reset();
  std::tie(parsed_ok, result_range) =
      p.parse_until(request_parser_type::headers_done, valid_http_request);
  EXPECT_EQ(true, parsed_ok);
  EXPECT_FALSE(boost::empty(result_range));
  parsed.assign(std::begin(result_range), std::end(result_range));
  std::cout << "PARSED: " << parsed << " [state:" << p.state() << "] "
            << std::endl;
}
