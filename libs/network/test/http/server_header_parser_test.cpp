// Copyright 2010 Dean Michael Berris.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_TEST_MODULE HTTP Server Header Parser Test
#include <boost/network/protocol/http/server.hpp>
#include <boost/config/warning_disable.hpp>
#include <boost/test/unit_test.hpp>
#include <string>
#include <iostream>

/** Synopsis
*
*  Test for Utf8 support in the asynchronous connection header parser
*  --------------------------------------------
*
*  This test checks for Utf8 support in the header parser
*  for asynchronous connection
*
*/

namespace tags = boost::network::tags;
namespace logic = boost::logic;
namespace fusion = boost::fusion;
using namespace boost::network::http;

BOOST_AUTO_TEST_CASE(async_connection_parse_headers) {
    request_header_narrow utf8_header = { "X-Utf8-Test-Header", "R\uc5abdolfs" };
    std::string valid_http_request;
    valid_http_request.append(utf8_header.name).append(": ").append(utf8_header.value).append("\r\n\r\n");
    std::vector<request_header_narrow> headers;
    parse_headers(valid_http_request, headers);
    std::vector<request_header_narrow>::iterator utf8_header_iterator = std::find_if(headers.begin(), headers.end(), [&utf8_header, &utf8_header_iterator](request_header_narrow& header)
    {
        if (header.name == utf8_header.name && header.value == utf8_header.value)
            return true;
        return false;
    });
    BOOST_CHECK(utf8_header_iterator != headers.end());
    std::cout << "utf8 header parsed, name: " << utf8_header_iterator->name << ", value: " << utf8_header_iterator->value << std::endl;
}