
//          Copyright Dean Michael Berris 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_TEST_MODULE message test
#include <boost/config/warning_disable.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/network.hpp>
#include <algorithm>


/**
 * Defines a set of template functions that can be used to test
 * generic code.
 */
namespace test_suite {
template <
    class Message
    >
void copy_constructor_test(Message instance,
                           const typename Message::string_type &h,
                           const typename Message::string_type &c) {
    typedef typename Message::string_type string;
    
    using namespace boost::network;
    instance << header(h, c);
    Message copy(instance);
    BOOST_CHECK_EQUAL (headers(copy).count(h), static_cast<std::size_t>(1));
    typename headers_range<Message>::type range = headers(copy)[h];
    BOOST_CHECK (begin(range) != end(range));
}

template <
    class Message
    >
void swap_test(Message instance,
               const typename Message::string_type &h,
               const typename Message::string_type &c) {
    instance << boost::network::header(h, c);
    Message other;
    swap(instance, other);
    BOOST_CHECK_EQUAL (headers(instance).count(h), static_cast<std::size_t>(0));
    BOOST_CHECK_EQUAL (headers(other).count(h), static_cast<std::size_t>(1));
}

template <
    class Message
    >
void headers_directive_test(Message instance,
                            const typename Message::string_type &h,
                            const typename Message::string_type &c) {
    using namespace boost::network;
    instance << header(h, c);
    
    BOOST_CHECK_EQUAL( headers(instance).count(h), static_cast<std::size_t>(1) );
    typename headers_range<Message>::type range = headers(instance)[h];
    BOOST_CHECK( begin(range) != end(range) );
}

template <
    class Message
    >
void body_directive_test(Message instance,
                         const typename Message::string_type &b) {
    using namespace boost::network;
    instance << body(b) ;

    // BOOST_CHECK_EQUAL( body(instance), b);
    BOOST_CHECK( body(instance) == b);
}

template <
    class Message
    >
void source_directive_test(Message instance,
                           const typename Message::string_type &s) {
    using namespace boost::network;
	instance << source(s) ;
	
	// BOOST_CHECK_EQUAL( source(instance), s);
	BOOST_CHECK( source(instance) == s);
}

template <
    class Message
    >
void destination_directive_test(Message instance,
                                const typename Message::string_type &d) {
    using namespace boost::network;
    instance << destination(d);

    // BOOST_CHECK_EQUAL( destination(instance), d);
    BOOST_CHECK( destination(instance) == d);
    
}

template <
    class Message
    >
void remove_header_directive_test(Message instance,
                                  const typename Message::string_type &h,
                                  const typename Message::string_type &c) {
	using namespace boost::network;
    instance << header(h, c)
             << remove_header(h);
	typename headers_range<Message>::type range = headers(instance);
	BOOST_CHECK ( begin(range) == end(range) );
}
}

BOOST_AUTO_TEST_CASE(copy_constructor_test) {
    test_suite::copy_constructor_test(boost::network::message(),
                                      "SOME_HEADER",
                                      "SOME_CONTENT");
    test_suite::copy_constructor_test(boost::network::wmessage(),
                                      L"SOME_HEADER",
                                      L"SOME_CONTENT");
}

BOOST_AUTO_TEST_CASE(swap_test) {
    test_suite::swap_test(boost::network::message(),
                          "SOME_HEADER",
                          "SOME_CONTENT");
    test_suite::swap_test(boost::network::wmessage(),
                          L"SOME_HEADER",
                          L"SOME_CONTENT");
}

BOOST_AUTO_TEST_CASE(header_directives_test) {
    test_suite::headers_directive_test(boost::network::message(),
                                       "SOME_HEADER",
                                       "SOME_CONTENT");
    test_suite::headers_directive_test(boost::network::wmessage(),
                                       L"SOME_HEADER",
                                       L"SOME_CONTENT");
}

BOOST_AUTO_TEST_CASE(body_directive_test) {
    test_suite::body_directive_test(boost::network::message(),
                                    "The quick brown fox jumps over the lazy dog.");
    test_suite::body_directive_test(boost::network::wmessage(),
                                    L"The quick brown fox jumps over the lazy dog.");
}

BOOST_AUTO_TEST_CASE(source_directive_test) {
    test_suite::source_directive_test(boost::network::message(),
                                      "Somewhere Out There");
    test_suite::source_directive_test(boost::network::wmessage(),
                                      L"Somewhere Out There");
}

BOOST_AUTO_TEST_CASE(destination_directive_test) {
    test_suite::destination_directive_test(boost::network::message(),
                                           "Somewhere Out There");
    test_suite::destination_directive_test(boost::network::wmessage(),
                                           L"Somewhere Out There");
}

BOOST_AUTO_TEST_CASE(remove_header_directive_test) {
    test_suite::remove_header_directive_test(boost::network::message(),
                                             "Header",
                                             "Value");
    test_suite::remove_header_directive_test(boost::network::wmessage(),
                                             L"Header",
                                             L"Value");
}

