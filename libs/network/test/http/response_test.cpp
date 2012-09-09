// Copyright 2012 Dean Michael Berris <dberris@google.com>.
// Copyright 2012 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_TEST_MODULE HTTP Client Response Test
#include <network/protocol/http/response.hpp>
#include <boost/test/unit_test.hpp>

namespace http = boost::network::http;

BOOST_AUTO_TEST_CASE(response_constructor_test) {
  http::response created;
}

BOOST_AUTO_TEST_CASE(response_value_semantics_test) {
  http::response original;
  http::response copy(original);
  http::response assigned;
  assigned = original;
  BOOST_CHECK(original == assigned);
  assigned.set_source("http://www.google.com/");
  BOOST_CHECK(original != assigned);
  std::swap(assigned, copy);
  BOOST_CHECK(assigned == original);
  BOOST_CHECK(copy != original);
  BOOST_CHECK(assigned != copy);
  original = copy;
  BOOST_CHECK(original == copy);
}

struct multimap_inserter {
  void operator()(std::string const &name, std::string const &value)  const {
    multimap_.insert(std::make_pair(name, value));
  }
  explicit multimap_inserter(std::multimap<std::string, std::string> &multimap)
  : multimap_(multimap)
  {}
  std::multimap<std::string,std::string> & multimap_;
};

BOOST_AUTO_TEST_CASE(response_setters_and_getters_test) {
  http::response response;
  response.set_source("http://www.google.com/");
  response.set_destination("127.0.0.1");
  response.append_header("Connection", "close");
  response.append_header("Content-Type", "text/plain");
  response.set_body("Hello, World!");
  response.set_status(200u);
  response.set_status_message("OK");
  response.set_version("HTTP/1.1");
  std::string source, destination, body, status_message, version;
  std::multimap<std::string, std::string> headers, expected_headers;
  expected_headers.insert(std::make_pair("Connection", "close"));
  expected_headers.insert(std::make_pair("Content-Type", "text/plain"));
  boost::uint16_t status;
  response.get_source(source);
  response.get_destination(destination);
  response.get_body(body);
  response.get_status_message(status_message);
  response.get_version(version);
  response.get_headers(multimap_inserter(headers));
  response.get_status(status);
  BOOST_CHECK_EQUAL(source, std::string("http://www.google.com/"));
  BOOST_CHECK_EQUAL(destination, std::string("127.0.0.1"));
  BOOST_CHECK_EQUAL(body, std::string("Hello, World!"));
  BOOST_CHECK_EQUAL(status, 200u);
  BOOST_CHECK_EQUAL(version, std::string("HTTP/1.1"));
  BOOST_CHECK(expected_headers == headers);
}
