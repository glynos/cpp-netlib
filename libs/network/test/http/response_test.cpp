// Copyright 2012 Dean Michael Berris <dberris@google.com>.
// Copyright 2012 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_TEST_MODULE HTTP Client Response Test
#include <boost/network/protocol/http/response.hpp>
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
