// Copyright 2012 Dean Michael Berris <dberris@google.com>.
// Copyright 2012 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_TEST_MODULE HTTP Request Test
#include <boost/network/protocol/http/request.hpp>
#include <boost/network/message/wrappers.hpp>
#include <boost/test/unit_test.hpp>

namespace http = boost::network::http;
namespace uri = boost::network::uri;
namespace net = boost::network;

BOOST_AUTO_TEST_CASE(request_construction) {
  http::request request;
  http::request other(request);
}

BOOST_AUTO_TEST_CASE(request_uri_test) {
  http::request request;
  request.set_uri("http://www.google.com/");
  http::request other(request);
  std::string original, copied;
  request.get_uri(original);
  other.get_uri(copied);
  BOOST_CHECK_EQUAL(std::string("http://www.google.com/"), original);
  BOOST_CHECK_EQUAL(original, copied);
}

BOOST_AUTO_TEST_CASE(request_basics_test) {
  http::request request;
  request.set_uri("http://www.google.com/");
  request.set_source("127.0.0.1");
  request.set_destination("http://www.google.com/");
  request.append_header("X-Referer", "http://cpp-netlib.github.com/");
  request.append_header("Connection", "close");
  request.append_body("The quick brown fox jumps over the lazy dog!");

  uri::uri uri_;
  std::string source_, destination_, body_;
  net::headers_wrapper::range_type headers_range = headers(request);
  request.get_uri(uri_);
  request.get_source(source_);
  request.get_destination(destination_);
  request.get_body(body_);

  BOOST_CHECK_EQUAL(uri_.string(), std::string("http://www.google.com/"));
  BOOST_CHECK_EQUAL(source_, std::string("127.0.0.1"));
  BOOST_CHECK_EQUAL(body_, std::string("The quick brown fox jumps over the lazy dog!"));
  BOOST_CHECK(!boost::empty(headers_range));
}
