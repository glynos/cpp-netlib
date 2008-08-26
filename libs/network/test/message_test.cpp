
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

namespace boost { namespace network {

    template <> struct headers_container< ::unit_test::tags::special> 
    { typedef std::map<std::string, std::string> type; };

	template <> struct string< ::unit_test::tags::special>
	{ typedef std::string type; };

}
}
    
BOOST_AUTO_TEST_CASE(constructor_test) {

    using namespace boost::network;
    message msg;

    basic_message<unit_test::tags::special> special_message;
}

BOOST_AUTO_TEST_CASE(copy_constructor_test) {
    using namespace boost::network;
    message msg;
    msg << header("SOME_HEADER", "SOME_CONTENT");
    message copy(msg);
    BOOST_CHECK_EQUAL (headers(copy).count("SOME_HEADER"), (size_t) 1);
    headers_range<message>::type range = headers(copy)["SOME_HEADER"];
    BOOST_CHECK (begin(range) != end(range));
}

BOOST_AUTO_TEST_CASE(swap_test) {
    boost::network::message msg;
    msg << boost::network::header("SOME_HEADER", "SOME CONTENT");
    boost::network::message other;
    swap(msg, other);
    BOOST_CHECK_EQUAL (headers(msg).count("SOME_HEADER"), (size_t) 0);
    BOOST_CHECK_EQUAL (headers(other).count("SOME_HEADER"), (size_t) 1);
}

BOOST_AUTO_TEST_CASE(header_directives_test) {
    using namespace boost::network;
    message msg;
    msg << header("SOME_HEADER", "SOME_CONTENT") ;
    
    BOOST_CHECK_EQUAL( headers(msg).count("SOME_HEADER") , (size_t) 1 );
    headers_range<message>::type range = headers(msg)["SOME_HEADER"];
    BOOST_CHECK( begin(range) != end(range) );
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
}

BOOST_AUTO_TEST_CASE(remove_header_directive_test) {
	using namespace boost::network;
	message msg;
	msg << header("Header", "Value")
		<< remove_header("Header");
	headers_range<message>::type range = headers(msg);
	BOOST_CHECK ( begin(range) == end(range) );
}

