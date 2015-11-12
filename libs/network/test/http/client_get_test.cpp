// Copyright 2010 Dean Michael Berris.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_TEST_MODULE HTTP Get Test
#include <boost/network/include/http/client.hpp>
#include <boost/test/unit_test.hpp>
#include "client_types.hpp"

namespace net = boost::network;
namespace http = boost::network::http;

BOOST_AUTO_TEST_CASE_TEMPLATE(http_client_get_test, client, client_types) {
  typename client::request request("http://cpp-netlib.org/");
  client client_;
  typename client::response response;
  BOOST_REQUIRE_NO_THROW(response = client_.get(request));
  try {
    auto data = body(response);
    std::cout << data;
  } catch (...) {
    BOOST_ASSERT(false);
  }
  BOOST_CHECK_EQUAL(response.version().substr(0, 7), std::string("HTTP/1."));
  BOOST_CHECK(response.status() == 200u ||
              (response.status() >= 300 && response.status() < 400));
}

#ifdef BOOST_NETWORK_ENABLE_HTTPS

BOOST_AUTO_TEST_CASE_TEMPLATE(https_client_get_test, client, client_types) {
  typename client::request request("https://www.github.com/");
  client client_;
  typename client::response response = client_.get(request);
  BOOST_CHECK(response.status() == 200 ||
              (response.status() >= 300 && response.status() < 400));
  try {
    auto data = body(response);
    std::cout << data;
  } catch (...) {
    BOOST_ASSERT(false);
  }
}

#endif

BOOST_AUTO_TEST_CASE_TEMPLATE(http_temp_client_get_test, client, client_types) {
  typename client::request request("http://cpp-netlib.org/");
  typename client::response response;
  BOOST_REQUIRE_NO_THROW(response = client().get(request));
  auto range = headers(response);
  BOOST_CHECK(!boost::empty(range));
  try {
    auto data = body(response);
    std::cout << data;
  } catch (...) {
    BOOST_ASSERT(false);
  }
  BOOST_CHECK_EQUAL(response.version().substr(0, 7), std::string("HTTP/1."));
  BOOST_CHECK(response.status() == 200u ||
              (response.status() >= 300 && response.status() < 400));
}
