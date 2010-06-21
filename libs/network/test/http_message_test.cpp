
// Copyright 2010 (c) Dean Michael Berris.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_TEST_MODULE HTTP message test
#include <boost/config/warning_disable.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/network/protocol/http/request.hpp>
#include <boost/network/protocol/http/response.hpp>
#include <boost/mpl/list.hpp>
// #include <boost/network/protocol/http/traits.hpp>
#include <algorithm>

using namespace boost::network;

typedef boost::mpl::list<
    tags::http_default_8bit_tcp_resolve
    , tags::http_default_8bit_udp_resolve
    , tags::http_keepalive_8bit_tcp_resolve
    , tags::http_keepalive_8bit_udp_resolve
> tag_types;

struct fixtures {
};

BOOST_FIXTURE_TEST_SUITE(http_message_test_suite, fixtures)

BOOST_AUTO_TEST_CASE_TEMPLATE (request_constructor_test, T, tag_types) {
    http::basic_request<T> request("http://boost.org");
    typedef http::basic_request<T>::string_type string_type;
    string_type host = http::host(request);
    boost::uint16_t port = http::port(request);
    string_type path = http::path(request);
    string_type query = http::query(request);
    string_type anchor = http::anchor(request);
    string_type protocol = http::protocol(request);
    BOOST_CHECK_EQUAL ( host , "boost.org" );
    BOOST_CHECK_EQUAL ( port , 80u );
    BOOST_CHECK_EQUAL ( path, "/" );
    BOOST_CHECK_EQUAL ( query, "" );
    BOOST_CHECK_EQUAL ( anchor, "" );
    BOOST_CHECK_EQUAL ( protocol, "http");
}

BOOST_AUTO_TEST_CASE_TEMPLATE (request_copy_constructor_test, T, tag_types) {
    http::basic_request<T> request("http://boost.org/handler.php");
    request << header("Content-Type", "text/plain")
            << body("Hello, World!")
        ;
    http::basic_request<T> copy(request);
    BOOST_CHECK_EQUAL ( copy.host(), request.host() );
    BOOST_CHECK_EQUAL ( copy.port(), request.port() );
    BOOST_CHECK_EQUAL ( request.path(), request.path() );
    BOOST_CHECK_EQUAL ( body(request), body(copy) );
}

BOOST_AUTO_TEST_CASE_TEMPLATE (request_assignment_test, T, tag_types) {
    http::basic_request<T> request("http://boost.org/handler.php");
    request << header("Content-Type", "text/plain")
            << body("Hello, World!")
        ;
    http::basic_request<T> copy;
    copy = request;
    BOOST_CHECK_EQUAL ( copy.host(), request.host() );
    BOOST_CHECK_EQUAL ( copy.port(), request.port() );
    BOOST_CHECK_EQUAL ( request.path(), request.path() );
    BOOST_CHECK_EQUAL ( body(request), body(copy) );
}

BOOST_AUTO_TEST_CASE_TEMPLATE (request_swap_test, T, tag_types) {
    boost::network::http::basic_request<T> request("http://boost.org/");
    boost::network::http::basic_request<T> other;
    swap(other, request); // ADL
    BOOST_CHECK_EQUAL ( request.host(), "" );
    BOOST_CHECK_EQUAL ( request.port(), 80u );
    BOOST_CHECK_EQUAL ( request.path(), "/" );
    BOOST_CHECK_EQUAL ( other.host(), "boost.org" );
    BOOST_CHECK_EQUAL ( other.port(), 80u );
    BOOST_CHECK_EQUAL ( other.path(), "/" );
}

BOOST_AUTO_TEST_CASE_TEMPLATE (response_constructor_test, T, tag_types) {
    http::basic_response<T> response;
    BOOST_CHECK_EQUAL ( body(response), std::string() );
}

BOOST_AUTO_TEST_CASE_TEMPLATE (response_copy_construct_test, T, tag_types) {
    using namespace http;
    http::basic_response<T> response;
    response << http::version("HTTP/1.1")
        << http::status(200u)
        << body("The quick brown fox jumps over the lazy dog")
        << http::status_message("OK")
        ;
    http::basic_response<T> copy(response);

    typename http::basic_response<T>::string_type 
        version_orig = version(response)
        , version_copy = version(copy);
    BOOST_CHECK_EQUAL ( version_orig, version_copy );
    boost::uint16_t status_orig = status(response)
        , status_copy = status(copy);
    BOOST_CHECK_EQUAL ( status_orig, status_copy );
    typename http::basic_response<T>::string_type
        status_message_orig = status_message(response)
        , status_message_copy = status_message(copy);
    BOOST_CHECK_EQUAL ( status_message_orig, status_message_copy );
    BOOST_CHECK_EQUAL ( body(response), body(copy) );
}

BOOST_AUTO_TEST_CASE_TEMPLATE (response_assignment_construct_test, T, tag_types) {
    http::basic_response<T> response;
    response << http::version("HTTP/1.1")
        << http::status(200)
        << http::status_message("OK")
        << body("The quick brown fox jumps over the lazy dog");
    http::basic_response<T> copy;
    copy = response;
    typedef http::basic_response<T>::string_type string_type;
    string_type version_orig = version(response)
        , version_copy = version(copy);
    BOOST_CHECK_EQUAL ( version_orig, version_copy );
    boost::uint16_t status_orig = status(response)
        , status_copy = status(copy);
    BOOST_CHECK_EQUAL ( status_orig, status_copy );
    string_type status_message_orig = status_message(response)
        , status_message_copy = status_message(copy);
    BOOST_CHECK_EQUAL ( status_message_orig, status_message_copy );
    BOOST_CHECK_EQUAL ( body(response), body(copy) );
}

BOOST_AUTO_TEST_CASE_TEMPLATE (response_swap_test, T, tag_types) {
    http::basic_response<T> response;
    response.version() = "HTTP/1.1";
    response.status() = 200;
    response.status_message() = "OK";
    response << boost::network::body("RESPONSE");
    boost::network::http::basic_response<T> swapped;
    BOOST_REQUIRE_NO_THROW(swap(response, swapped));
    BOOST_CHECK_EQUAL ( response.version(), std::string() );
    BOOST_CHECK_EQUAL ( response.status(), 0u );
    BOOST_CHECK_EQUAL ( response.status_message(), std::string() );
    BOOST_CHECK_EQUAL ( boost::network::body(response), std::string() );
    BOOST_CHECK_EQUAL ( swapped.version(), std::string("HTTP/1.1") );
    BOOST_CHECK_EQUAL ( swapped.status(), 200u );
    BOOST_CHECK_EQUAL ( swapped.status_message(), std::string("OK") );
    BOOST_CHECK_EQUAL ( boost::network::body(swapped), std::string("RESPONSE") );
}

BOOST_AUTO_TEST_SUITE_END()
