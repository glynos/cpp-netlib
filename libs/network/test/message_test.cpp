
//          Copyright Dean Michael Berris 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_TEST_MODULE message test
#include <boost/config/warning_disable.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/network/message.hpp>
#include <algorithm>

using namespace boost::network;

/**
 * Defines a set of template functions that can be used to test
 * generic code.
 */

BOOST_AUTO_TEST_CASE(copy_constructor_test) {
    message instance;
    instance << header("name", "value");
    message copy(instance);
    BOOST_CHECK_EQUAL(headers(copy).count("name"), static_cast<std::size_t>(1));
    message::headers_range range = headers(copy)["name"];
    BOOST_CHECK (!boost::empty(range));
}

BOOST_AUTO_TEST_CASE(swap_test) {
    message instance;
    instance << header("name", "value");
    message other;
    swap(instance, other);
    BOOST_CHECK_EQUAL (headers(instance).count("name"), static_cast<std::size_t>(0));
    BOOST_CHECK_EQUAL (headers(other).count("name"), static_cast<std::size_t>(1));
}

BOOST_AUTO_TEST_CASE(headers_directive_test) {
    message instance;
    instance << header("name", "value");
    BOOST_CHECK_EQUAL ( headers(instance).count("name"), static_cast<std::size_t>(1) );
    message::headers_range range = headers(instance)["name"];
    BOOST_CHECK (boost::begin(range) != boost::end(range));
}

BOOST_AUTO_TEST_CASE(body_directive_test) {
    message instance;
    instance << ::boost::network::body("body");
    std::string body_string = body(instance);
    BOOST_CHECK ( body_string == "body" );
}

BOOST_AUTO_TEST_CASE(source_directive_test) {
    message instance;
    instance << ::boost::network::source("source");
    std::string source_string = source(instance);
    BOOST_CHECK ( source_string == "source" );
}

BOOST_AUTO_TEST_CASE(destination_directive_test) {
    message instance;
    instance << destination("destination");
    std::string const & destination_ = destination(instance);
    BOOST_CHECK ( destination_ == "destination" );
}

BOOST_AUTO_TEST_CASE(remove_header_directive_test) {
    message instance;
    instance << header("name", "value")
        << remove_header("name");
    message::headers_range range = headers(instance)["name"];
    BOOST_CHECK ( boost::begin(range) == boost::end(range) );
}
