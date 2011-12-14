
// Copyright 2010 Dean Michael Berris. 
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_TEST_MODULE HTTP Client Get Different Port Test
#include <boost/network/include/http/client.hpp>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(http_get_test_different_port) {
  using namespace boost::network::http;
  request request_("http://www.boost.org:80/");
  client client_;
  response response_ = client_.get(request_);
  boost::network::headers_wrapper::range_type headers_ = headers(response_)["Content-Type"];
  BOOST_CHECK( boost::begin(headers_) != boost::end(headers_) );
  BOOST_CHECK( body(response_).size() > 0 );
}

