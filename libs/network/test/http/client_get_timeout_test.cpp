
// Copyright 2010 Dean Michael Berris. 
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_TEST_MODULE HTTP Client Get Timeout Test
#include <boost/network/include/http/client.hpp>
#include <boost/test/unit_test.hpp>
#include "tag_types.hpp"

namespace http = boost::network::http;

BOOST_AUTO_TEST_CASE_TEMPLATE(http_get_test_timeout_1_0, T, tag_types) {
    typedef http::basic_client<T, 1, 0> client;
    typename client::request request("http://localhost:12121/");
    typename client::response response_;
    client client_;
    boost::uint16_t port_ = port(request);
    typename client::response::string_type temp;
    BOOST_CHECK_EQUAL ( 12121, port_ );
    BOOST_CHECK_THROW ( response_ = client_.get(request); temp = body(response_); , std::exception );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(http_get_test_timeout_1_1, T, tag_types) {
    typedef http::basic_client<T, 1, 1> client_type;
    typename client_type::request request("http://localhost:12121/");
    typename client_type::response response_;
    boost::uint16_t port_ = port(request);
    typename client_type::response::string_type temp;
    BOOST_CHECK_EQUAL ( 12121, port_ );
    client_type client_;
    BOOST_CHECK_THROW ( response_ = client_.get(request); temp = body(response_); , std::exception );
}
