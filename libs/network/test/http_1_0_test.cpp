
//          Copyright Dean Michael Berris 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_TEST_MODULE http 1.0 test
#include <boost/test/unit_test.hpp>
#include <boost/network.hpp>
#include <iostream>

BOOST_AUTO_TEST_CASE(http_get_test) {
    using namespace boost::network;
    http::request request("http://www.boost.org/");
    http::client client_;
    http::response response_;
    response_ = client_.get(request);
    headers_range<http::response>::type range = headers(response_)["Content-Type"];
	BOOST_CHECK ( begin(range) != end(range) );
    BOOST_CHECK ( body(response_).size() != 0 );
}

BOOST_AUTO_TEST_CASE(http_get_test_different_port) {
    using namespace boost::network;
    http::request request("http://www.boost.org:80/");
    http::client client_;
    http::response response_;
    response_ = client_.get(request);
    headers_range<http::response>::type range = headers(response_)["Content-Type"];
    BOOST_CHECK ( begin(range) != end(range) );
    BOOST_CHECK ( body(response_).size() != 0 );
}

BOOST_AUTO_TEST_CASE(http_get_test_timeout) {
    using namespace boost::network;
    http::request request("http://localhost:12121/");
    http::client client_;
    http::response response_;
    BOOST_CHECK_THROW ( response_ = client_.get(request), boost::system::system_error );
}

BOOST_AUTO_TEST_CASE(http_get_details) {
	using namespace boost::network;
	http::request request("http://www.boost.org/");
	http::client client_;
	http::response response_;
	BOOST_CHECK_NO_THROW ( response_ = client_.get(request) );
	BOOST_CHECK_EQUAL ( response_.version().substr(0,7), std::string("HTTP/1.") );
	BOOST_CHECK_EQUAL ( response_.status(), 200u );
	BOOST_CHECK_EQUAL ( response_.status_message(), std::string("OK") );
}

BOOST_AUTO_TEST_CASE(http_cached_resolve) {
	using namespace boost::network;
	http::request request("http://www.boost.org");
	http::request other_request("http://www.boost.org/users/license.html");
	http::client client_(http::client::cache_resolved);
	http::response response_;
	BOOST_CHECK_NO_THROW ( response_ = client_.get(request) );
	BOOST_CHECK_NO_THROW ( response_ = client_.get(other_request) );
}

BOOST_AUTO_TEST_CASE(http_redirection_test) {
	using namespace boost::network;
	http::request request("http://boost.org");
	http::client client_(http::client::follow_redirect);
	http::response response_;
	BOOST_CHECK_NO_THROW ( response_ = client_.get(request) );
	BOOST_CHECK_EQUAL ( response_.status(), 200u );
}
