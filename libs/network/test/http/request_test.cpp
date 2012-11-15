// Copyright 2012 Dean Michael Berris <dberris@google.com>.
// Copyright 2012 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifdef BUILD_SHARED_LIBS
# define BOOST_TEST_DYN_LINK
#endif
#define BOOST_TEST_MODULE HTTP Request Test
#include <network/protocol/http/request.hpp>
#include <network/protocol/http/message/wrappers.hpp>
#include <network/message/wrappers.hpp>
#include <boost/test/unit_test.hpp>
#include <network/uri/uri_io.hpp>

namespace http = network::http;
namespace net = network;

BOOST_AUTO_TEST_CASE(request_construction) {
  http::request request;
  http::request other(request);
}

BOOST_AUTO_TEST_CASE(request_value_semantics) {
  // First let's default construct a request.
  http::request original;
  // Next let's copy the request.
  http::request copy(original);
  // Next let's compare the requests.
  BOOST_CHECK(original == copy);
  // Next let's assign the original to another request.
  http::request assigned;
  assigned = original;
  // Next we modify the assigned object and make sure it's not the same as the
  // original.
  assigned.set_uri("http://www.google.com/");
  assigned.set_source("127.0.0.1");
  assigned.set_destination("http://www.google.com/");
  assigned.append_header("Connection", "close");
  assigned.set_body("Hello, world!");
  BOOST_CHECK(original != assigned);
  // Next we swap the assigned and copy.
  std::swap(assigned, copy);
  BOOST_CHECK(copy != assigned);
  BOOST_CHECK(copy != original);
  BOOST_CHECK(original == assigned);
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

  // Now we test the bare uri instance with accessing using the request
  // convenience wrapper.
  network::uri uri_;
  request.get_uri(uri_);
  std::string host_ = http::host(request);
  BOOST_CHECK(network::valid(uri_));
  BOOST_CHECK_EQUAL(std::string("www.google.com"), host_);
  BOOST_CHECK_EQUAL(uri_.host(), host_);
  BOOST_CHECK_EQUAL(std::string("www.google.com"), uri_.host());
}

BOOST_AUTO_TEST_CASE(request_url_constructor_test) {
  http::request request("http://www.google.com/");
  http::request other;
  other.set_uri("http://www.google.com/");
  network::uri original, other_uri;
  request.get_uri(original);
  other.get_uri(other_uri);
  BOOST_CHECK_EQUAL(original, other_uri);

  // Now test the directives..
  network::uri directive_original = http::uri(request);
  BOOST_CHECK_EQUAL(original, directive_original);
}

BOOST_AUTO_TEST_CASE(request_basics_test) {
  http::request request;
  request.set_uri("http://www.google.com/");
  request.set_source("127.0.0.1");
  request.set_destination("destination!");
  request.append_header("X-Referer", "http://cpp-netlib.github.com/");
  request.append_header("Connection", "close");
  request.append_body("The quick brown fox jumps over the lazy dog!");

  network::uri uri_;
  std::string source_, destination_, body_;
  net::headers_wrapper::container_type const &headers_ = headers(request);
  request.get_uri(uri_);
  request.get_source(source_);
  request.get_destination(destination_);
  request.get_body(body_);

  BOOST_CHECK_EQUAL(uri_.string(), std::string("http://www.google.com/"));
  BOOST_CHECK_EQUAL(source_, std::string("127.0.0.1"));
  BOOST_CHECK_EQUAL(destination_, std::string("destination!"));
  BOOST_CHECK_EQUAL(body_, std::string("The quick brown fox jumps over the lazy dog!"));
  BOOST_CHECK(!boost::empty(headers_));
}
