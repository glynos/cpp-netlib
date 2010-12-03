
// Copyright 2010 Dean Michael Berris. 
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_TEST_MODULE HTTP 1.0 Client Constructor Test
#include <boost/network/include/http/client.hpp>
#include <boost/test/unit_test.hpp>
#include "tag_types.hpp"

namespace http = boost::network::http;

BOOST_AUTO_TEST_CASE_TEMPLATE(http_1_0_constructor_test, T, tag_types) {
    typedef http::basic_client<T, 1, 0> client;
    client instance;
    boost::asio::io_service io_service;
    client instance2(io_service);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(http_1_1_constructor_test, T, tag_types) {
    typedef http::basic_client<T, 1, 1> client;
    client instance;
    boost::asio::io_service io_service;
    client instance2(io_service);
}

