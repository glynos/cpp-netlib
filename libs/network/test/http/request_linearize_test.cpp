
// Copyright 2010 Dean Michael Berris.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_TEST_MODULE HTTP Request Linearize Test
#include <network/protocol/http/request.hpp>
#include <network/protocol/http/algorithms/linearize.hpp>
#include <boost/test/unit_test.hpp>
#include <iostream>

namespace http = boost::network::http;
namespace net  = boost::network;

BOOST_AUTO_TEST_CASE(linearize_request) {
    http::request request("http://www.boost.org");
    linearize(request, "GET", 1, 0, std::ostream_iterator<char>(std::cout));
    linearize(request, "GET", 1, 1, std::ostream_iterator<char>(std::cout));
}

