// Copyright 2010 Dean Michael Berris. 
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_TEST_MODULE HTTP 1.0 Get Test
#include <network/include/http/client.hpp>
#include <boost/test/unit_test.hpp>

namespace net = boost::network;
namespace http = boost::network::http;

BOOST_AUTO_TEST_CASE(http_client_get_test) {
    http::client::request request("http://www.google.com/");
    request << net::header("Connection", "close");
    http::client client_;
    http::client::response response;
    BOOST_REQUIRE_NO_THROW ( response = client_.get(request) );
    std::multimap<std::string, std::string> headers_ = net::headers(response);
    BOOST_CHECK ( !boost::empty(headers_) );
    BOOST_REQUIRE_NO_THROW ( BOOST_CHECK ( body(response).size() != 0 ) );
    std::string version_, status_message_;
    response.get_version(version_);
    uint16_t status_;
    response.get_status(status_);
    response.get_status_message(status_message_);
    BOOST_CHECK_EQUAL ( version_.substr(0,7), "HTTP/1.");
    BOOST_CHECK_EQUAL ( status_, 302u );
    BOOST_CHECK_EQUAL ( status_message_, std::string("Found") );
}

#ifdef BOOST_NETWORK_ENABLE_HTTPS

BOOST_AUTO_TEST_CASE(https_client_get_test) {
    http::client::request request("https://www.google.com");
    request << net::header("Connection", "close");
    http::client client_;
    http::client::response response;
    BOOST_REQUIRE_NO_THROW ( response = client_.get(request) );
    std::multimap<std::string, std::string> headers_ = net::headers(response);
    BOOST_CHECK ( !boost::empty(headers_) );
    BOOST_REQUIRE_NO_THROW ( BOOST_CHECK ( body(response).size() != 0 ) );
    std::string version_, status_message_;
    response.get_version(version_);
    uint16_t status_;
    response.get_status(status_);
    response.get_status_message(status_message_);
    BOOST_CHECK_EQUAL ( version_.substr(0,7), "HTTP/1.");
    BOOST_CHECK_EQUAL ( status_, 302u );
    BOOST_CHECK_EQUAL ( status_message_, std::string("Found") );
}

#endif
