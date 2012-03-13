// Copyright 2012 Dean Michael Berris <dberris@google.com>.
// Copyright 2012 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_TEST_MODULE HTTP Request Test
#include <boost/network/protocol/http/request.hpp>
#include <boost/test/unit_test.hpp>

namespace http = boost::network::http;

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
