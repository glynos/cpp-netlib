
//          Copyright Dean Michael Berris 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_TEST_MODULE message test
#include <boost/test/unit_test.hpp>
#include <boost/network.hpp>
#include <algorithm>


namespace unit_test {
struct tags {
    struct special { };
};
}
    
BOOST_AUTO_TEST_CASE(constructor_test) {

    using namespace boost::network;
    message msg;

    basic_message<unit_test::tags::special> special_message;
}

BOOST_AUTO_TEST_CASE(header_directives_test) {
    using namespace boost::network;
    message msg;
    msg << header("SOME_HEADER", "SOME_CONTENT") ;
    
    BOOST_CHECK_EQUAL( headers(msg).count("SOME_HEADER") , (size_t) 1 );
    headers_range<message>::type range = headers(msg)["SOME_HEADER"];
    BOOST_CHECK( range.first != range.second );
}

BOOST_AUTO_TEST_CASE(body_directive_test) {
    using namespace boost::network;
    message msg;
    msg << body("The quick brown fox jumps over the lazy dog.") ;

    BOOST_CHECK_EQUAL( body(msg), "The quick brown fox jumps over the lazy dog.");
}

BOOST_AUTO_TEST_CASE(source_directive_test) {
	using namespace boost::network;
	message msg;
	msg << source("Somewhere Out There") ;
	
	BOOST_CHECK_EQUAL( source(msg), "Somewhere Out There");
}

BOOST_AUTO_TEST_CASE(destination_directive_test) {
    using namespace boost::network;
    message msg;
    msg << destination("Somewhere Out There");

    BOOST_CHECK_EQUAL( destination(msg), "Somewhere Out There");
};
