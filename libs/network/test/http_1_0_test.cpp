
//          Copyright Dean Michael Berris 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_TEST_MODULE http 1.0 test
#include <boost/test/unit_test.hpp>
#include <boost/network.hpp>
#include <iostream>

BOOST_AUTO_TEST_CASE(constructor_test) {
    using namespace boost::network;
    http::request request("http://boost.org");
    BOOST_CHECK_EQUAL ( request.host() , "boost.org" );
    BOOST_CHECK_EQUAL ( request.port() , 80u );
    BOOST_CHECK_EQUAL ( request.path() , "" );
}

BOOST_AUTO_TEST_CASE(http_get_test) {
    using namespace boost::network;
    http::request request("http://boost.org/");    
    http::client client_;
    http::response response_;
    response_ = client_.get(request);
    headers_range<http::response>::type range = headers(response_)["Content-Length"];
    BOOST_CHECK ( range.first != range.second );
    BOOST_CHECK ( body(response_).size() != 0 );
};
