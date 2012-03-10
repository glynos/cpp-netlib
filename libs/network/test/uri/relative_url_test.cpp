//            Copyright (c) Glyn Matthews 2012.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_TEST_MODULE Relative URL Test
#include <boost/config/warning_disable.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/network/uri/uri.hpp>
#include <boost/network/uri/uri_io.hpp>

using namespace boost::network;

BOOST_AUTO_TEST_CASE(relative_uri_test) {
    uri::uri instance("www.example.com/");
    BOOST_REQUIRE(uri::valid(instance));
    BOOST_CHECK(uri::is_absolute(instance));
    BOOST_CHECK_EQUAL(uri::path(instance), "www.example.com/");
}
