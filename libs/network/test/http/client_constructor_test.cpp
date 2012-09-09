
// Copyright 2010 Dean Michael Berris. 
// Copyright 2012 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_TEST_MODULE HTTP 1.0 Client Constructor Test
#include <network/include/http/client.hpp>
#include <boost/test/unit_test.hpp>

namespace http = boost::network::http;

BOOST_AUTO_TEST_CASE(http_client_constructor_test) {
  // Here's the simplest way to construct a client.
  http::client instance;
  
  // The next way we're supporting is actually to construct an options object
  // that allows you to set options. This class replaces the Boost.Parameter
  // based approach to a much simpler model that scales better.
  http::client_options options;
  boost::asio::io_service io_service;
  options.io_service(&io_service)
         .follow_redirects()
         .cache_resolved()
         .add_openssl_certificate_path("/dev/zero")
         .add_openssl_verify_path("/dev/null");
  http::client instance2(options);
}
