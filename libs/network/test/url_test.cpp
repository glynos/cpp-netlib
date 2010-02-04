
// Copyright 2009 Dean Michael Berris, Jeroen Habraken.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt of copy at
// http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_TEST_MODULE URL Test
#include <boost/config/warning_disable.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/network/uri.hpp>
#include <algorithm>


namespace test_suite {
template <
    class Uri
    >
void constructor_test(Uri instance,
                      const typename Uri::string_type &scheme_,
                      const typename Uri::string_type &rest_) {
    BOOST_CHECK(scheme(instance) == scheme_);
    // BOOST_CHECK(rest(instance) == rest_);
    BOOST_CHECK(valid(instance));
}

template <
    class Uri
    >
void copy_constructor_test(Uri instance) {
    Uri copy = instance;
    BOOST_CHECK(scheme(instance) == scheme(copy));
    // BOOST_CHECK(rest(instance) == rest(copy));
}

template <
    class Uri
    >
void assignment_test(Uri instance) {
    Uri copy;
    copy = instance;
    BOOST_CHECK(scheme(instance) == scheme(copy));
    // BOOST_CHECK(rest(instance) == rest(copy));
}

template <
    class Uri
    >
void http_full_uri_test(Uri instance,
                        const typename Uri::string_type &scheme_,
                        const typename Uri::string_type &rest_,
                        const typename Uri::string_type &user_info_,
                        const typename Uri::string_type &host_,
                        boost::uint32_t port_,
                        const typename Uri::string_type &path_,
                        const typename Uri::string_type &query_,
                        const typename Uri::string_type &fragment_) {
    using namespace boost::network::uri;
        
    BOOST_CHECK(scheme(instance) == scheme_);
    // BOOST_CHECK(rest(instance) == rest_);
    BOOST_CHECK(user_info(instance) == user_info_);
    BOOST_CHECK(host(instance) == host_);
    BOOST_CHECK(port(instance) == port_);
    BOOST_CHECK(path(instance) == path_);
    BOOST_CHECK(query(instance) == query_);
    BOOST_CHECK(fragment(instance) == fragment_);
    BOOST_CHECK(valid(instance));
}

template <
    class Uri
    >
void http_simple_uri_test(Uri instance,
                          const typename Uri::string_type &scheme_,
                          const typename Uri::string_type &rest_,
                          const typename Uri::string_type &user_info_,
                          const typename Uri::string_type &host_,
                          boost::uint32_t port_,
                          const typename Uri::string_type &path_,
                          const typename Uri::string_type &query_,
                          const typename Uri::string_type &fragment_) {
    BOOST_CHECK(scheme(instance) == scheme_);
    // BOOST_CHECK(rest(instance) == rest_);
    BOOST_CHECK(user_info(instance) == user_info_);
    BOOST_CHECK(host(instance) == host_);
    BOOST_CHECK(port(instance) == port_);
    BOOST_CHECK(path(instance) == path_);
    BOOST_CHECK(query(instance) == query_);
    BOOST_CHECK(fragment(instance) == fragment_);
    BOOST_CHECK(valid(instance));
}

template <
    class Uri
    >
void https_simple_uri_test(Uri instance,
                           const typename Uri::string_type &scheme_,
                           boost::uint32_t port_) {
    using namespace boost::network::uri;
    
    BOOST_CHECK(scheme(instance) == scheme_);
    BOOST_CHECK(port(instance) == port_);
}
    
template <
    class Uri
    >
void http_with_port_test(Uri instance,
                         const typename Uri::string_type &host_,
                         boost::uint32_t port_) {
    using namespace boost::network::uri;

    BOOST_CHECK(host(instance) == host_);
    BOOST_CHECK(port(instance) == port_);
    BOOST_CHECK(valid(instance));
}

template <
    class Uri
    >
void uri_with_spaces_should_fail(Uri instance) {
    using namespace boost::network::uri;
    
    // I don't know if it's a good idea to test that host and port are
    // set if the URI is invalid.
    // BOOST_CHECK_EQUAL(host(instance), std::string("www.boost.org"));
    // BOOST_CHECK_EQUAL(port(instance), 80u);
    BOOST_CHECK(!valid(instance));
}

template <
    class Uri
    >
void http_with_invalid_host_should_fail(Uri instance) {
    using namespace boost::network::uri;
    BOOST_CHECK(!valid(instance));
}

template <
    class Uri
    >
void http_with_invalid_scheme_should_fail(Uri instance) {
    using namespace boost::network::uri;
    BOOST_CHECK(!valid(instance));
}

} // namespace test_suite

BOOST_AUTO_TEST_CASE(constructor_test) {
    test_suite::constructor_test(
        boost::network::uri::uri("http://www.boost.org/"),
        "http", "//www.boost.org/");
    test_suite::constructor_test(
        boost::network::uri::wuri(L"http://www.boost.org/"),
        L"http", L"//www.boost.org/");
    test_suite::constructor_test(
        boost::network::uri::uri("http://129.79.245.252/"),
        "http", "//www.boost.org/");
    test_suite::constructor_test(
        boost::network::uri::uri("news:comp.infosystems.www.servers.unix"),
        "news", "comp.infosystems.www.servers.unix");
}

BOOST_AUTO_TEST_CASE(copy_constructor_test) {
    test_suite::copy_constructor_test(
        boost::network::uri::uri("http://www.boost.org/"));
    test_suite::copy_constructor_test(
        boost::network::uri::wuri(L"http://www.boost.org/"));
}

BOOST_AUTO_TEST_CASE(assignment_test) {
    test_suite::assignment_test(
        boost::network::uri::uri("http://www.boost.org/"));
    test_suite::assignment_test(
        boost::network::uri::wuri(L"http://www.boost.org/"));
}

BOOST_AUTO_TEST_CASE(http_full_uri_test) {
    test_suite::http_full_uri_test(
        boost::network::uri::http::uri(
            "http://user:password@www.boost.org/path?query#fragment"),
        "http", "//user:password@www.boost.org/path?query#fragment",
        "user:password", "www.boost.org", 80u, "/path", "query", "fragment");
}

BOOST_AUTO_TEST_CASE(http_simple_uri_test) {
    test_suite::http_simple_uri_test(
        boost::network::uri::http::uri("http://www.boost.org/"),
        "http", "//www.boost.org/", "", "www.boost.org", 80u, "/", "", "");
}

BOOST_AUTO_TEST_CASE(https_simple_uri_test) {
    test_suite::https_simple_uri_test(
        boost::network::uri::http::uri("https://www.boost.org/"),
        "https", 443u);
}

BOOST_AUTO_TEST_CASE(http_with_port_test) {
    test_suite::http_with_port_test(
        boost::network::uri::http::uri("http://www.boost.org:80/"),
        "www.boost.org", 80u);
}

BOOST_AUTO_TEST_CASE(uri_with_spaces_should_fail) {
    test_suite::uri_with_spaces_should_fail(
        boost::network::uri::uri("http://www.boost.org /"));
    test_suite::uri_with_spaces_should_fail(
        boost::network::uri::wuri(L"http://www.boost.org /"));
}

BOOST_AUTO_TEST_CASE(http_with_invalid_host_should_fail) {
    test_suite::http_with_invalid_host_should_fail(
        boost::network::uri::http::uri("http://www-.boost.org/"));
}

BOOST_AUTO_TEST_CASE(http_with_invalid_scheme_should_fail) {
    boost::network::uri::http::uri uri("ftp://ftp.boost.org/");
    BOOST_CHECK(!valid(uri));
}
