
// Copyright 2010 Dean Michael Berris. 
// Copyright 2012 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_TEST_MODULE HTTP Client Get Different Port Test
#include <network/include/http/client.hpp>
#include <boost/test/unit_test.hpp>

namespace http = boost::network::http;
namespace net = boost::network;

BOOST_AUTO_TEST_CASE(http_get_test_different_port) {
  http::request request_("http://www.boost.org:80/");
  http::client client_;
  http::response response_ = client_.get(request_);
  net::headers_wrapper::container_type const &headers_ = headers(response_);
  BOOST_CHECK( !headers_.empty() );
  BOOST_CHECK( body(response_).size() > 0 );
}

