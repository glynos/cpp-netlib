// Copyright 2010 Dean Michael Berris.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_TEST_MODULE HTTP Incremental Request Parser Test
#include <boost/config/warning_disable.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/network/protocol/http/server/request_parser.hpp>
#include <boost/network/tags.hpp>
#include <boost/range.hpp>
#include <boost/logic/tribool.hpp>
#include <string>
#include <iostream>

/** Synopsis
 *
 *  Test for the HTTP Request Incremental Parser
 *  --------------------------------------------
 *
 *  In this test we fully intend to specify how an incremental HTTP request 
 *  parser should be used. This follows the HTTP Response Incremental Parser
 *  example, and models the Incremental Parser Concept.
 *
 */

namespace tags = boost::network::tags;
namespace logic = boost::logic;
namespace fusion = boost::fusion;
using namespace boost::network::http;

BOOST_AUTO_TEST_CASE(incremental_parser_constructor) {
    request_parser<tags::default_string> p; // default constructible
}


