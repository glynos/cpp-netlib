
//          Copyright Dean Michael Berris 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_TEST_MODULE message test
#include <boost/test/unit_test.hpp>
#include <boost/network.hpp>
#include <algorithm>

BOOST_AUTO_TEST_CASE(constructor_test) {

    struct tags {
        struct special { };
    };

    using namespace boost::network;
    message msg;

    basic_message<tags::special> special_message;
}

BOOST_AUTO_TEST_CASE(header_directives_test) {
    using namespace boost::network;
    message msg;
    msg << header("SOME_HEADER", "SOME_CONTENT") ;
    
    BOOST_CHECK_EQUAL( headers(msg).count("SOME_HEADER") , 1 );
    headers_range<message>::type range = headers(msg)["SOME_HEADER"];
    BOOST_CHECK( range.first != range.second );
}
