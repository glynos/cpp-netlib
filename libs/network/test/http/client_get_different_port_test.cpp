
// Copyright 2010 Dean Michael Berris. 
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_TEST_MODULE HTTP Client Get Different Port Test
#include <boost/network/include/http/client.hpp>
#include <boost/test/unit_test.hpp>
#include "tag_types.hpp"

namespace net = boost::network;
namespace http = boost::network::http;

BOOST_AUTO_TEST_CASE_TEMPLATE(http_get_test_different_port_1_0, T, tag_types) {
    typedef http::basic_client<T, 1, 0> client;
    typename client::request request("http://www.boost.org:80/");
    client client_;
    typename client::response response_ = client_.get(request);
    typename net::headers_range<typename http::basic_response<T> >::type range = headers(response_)["Content-Type"];
    BOOST_CHECK ( boost::begin(range) != boost::end(range) );
    BOOST_CHECK ( body(response_).size() != 0 );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(http_get_test_different_port_1_1, T, tag_types) {
    typedef http::basic_client<T, 1, 1> client;
    typename client::request request("http://www.boost.org:80/");
    typename client::response response_;
    client client_;
    BOOST_CHECK_NO_THROW ( response_ = client_.get(request) );
    typename net::headers_range<typename client::response >::type range = headers(response_)["Content-Type"];
    BOOST_CHECK ( boost::begin(range) != boost::end(range) );
    BOOST_CHECK ( body(response_).size() != 0 );
}

