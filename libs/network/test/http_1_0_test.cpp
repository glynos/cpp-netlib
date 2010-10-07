
//          Copyright Dean Michael Berris 2007-2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_TEST_MODULE http 1.0 test
#include <boost/config/warning_disable.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/network/include/http/client.hpp>
#include <iostream>
#include <boost/mpl/list.hpp>

using namespace boost::network;

typedef boost::mpl::list<
    tags::http_default_8bit_tcp_resolve
    , tags::http_default_8bit_udp_resolve
    , tags::http_async_8bit_udp_resolve
> tag_types;

BOOST_AUTO_TEST_CASE_TEMPLATE(http_get_test, T, tag_types) {
    typedef http::basic_client<T, 1, 0> client;
    typename client::request request("http://www.boost.org/");
    client client_;
    typename client::response response_ = client_.get(request);
    typename headers_range<typename http::basic_response<T> >::type range = headers(response_)["Content-Type"];
    BOOST_CHECK ( boost::begin(range) != boost::end(range) );
    BOOST_CHECK ( body(response_).size() != 0 );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(http_get_test_different_port, T, tag_types) {
    typedef http::basic_client<T, 1, 0> client;
    typename client::request request("http://www.boost.org:80/");
    client client_;
    typename client::response response_ = client_.get(request);
    typename headers_range<typename http::basic_response<T> >::type range = headers(response_)["Content-Type"];
    BOOST_CHECK ( boost::begin(range) != boost::end(range) );
    BOOST_CHECK ( body(response_).size() != 0 );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(http_get_test_timeout, T, tag_types) {
    typedef http::basic_client<T, 1, 0> client;
    typename client::request request("http://localhost:12121/");
    client client_;
    typename client::response response_;
    boost::uint16_t port_ = port(request);
    BOOST_CHECK_EQUAL ( 12121, port_ );
    BOOST_CHECK_THROW ( response_ = client_.get(request), boost::system::system_error );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(http_get_details, T, tag_types) {
    typedef http::basic_client<T, 1, 0> client;
    typename client::request request("http://www.boost.org/");
    client client_;
    typename client::response response_;
    BOOST_CHECK_NO_THROW ( response_ = client_.get(request) );
    BOOST_CHECK_EQUAL ( response_.version().substr(0,7), std::string("HTTP/1.") );
    BOOST_CHECK_EQUAL ( response_.status(), 200u );
    BOOST_CHECK_EQUAL ( response_.status_message(), std::string("OK") );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(http_cached_resolve, T, tag_types) {
    typedef http::basic_client<T, 1, 0> client;
    typename client::request request("http://www.boost.org");
    typename client::request other_request("http://www.boost.org/users/license.html");
    client client_(client::cache_resolved);
    typename client::response response_;
    BOOST_CHECK_NO_THROW ( response_ = client_.get(request) );
    BOOST_CHECK_NO_THROW ( response_ = client_.get(other_request) );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(http_redirection_test, T, tag_types) {
    typedef http::basic_client<T, 1, 0> client;
    typename client::request request("http://boost.org");
    client client_(client::follow_redirect);
    typename client::response response_;
    BOOST_CHECK_NO_THROW ( response_ = client_.get(request) );
    BOOST_CHECK_EQUAL ( response_.status(), 200u );
}

