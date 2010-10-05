
//      Copyright Dean Michael Berris 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//      http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_TEST_MODULE http 1.1 test
#include <boost/config/warning_disable.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/network/include/http/client.hpp>
#include <iostream>
#include <boost/mpl/list.hpp>

using namespace boost::network;

typedef boost::mpl::list<
    tags::http_default_8bit_tcp_resolve, 
    tags::http_default_8bit_udp_resolve, 
    tags::http_keepalive_8bit_tcp_resolve, 
    tags::http_keepalive_8bit_udp_resolve,
    tags::http_async_8bit_tcp_resolve,
    tags::http_async_8bit_udp_resolve
> tag_types;

BOOST_AUTO_TEST_CASE_TEMPLATE(http_get_test, T, tag_types) {
    typedef http::basic_client<T, 1, 1> client_type;
    typename client_type::request request("http://www.boost.org/");
    typename client_type::response response_;
    client_type client_;
    BOOST_CHECK_NO_THROW ( response_ = client_.get(request) );
    typename headers_range<typename client_type::response>::type range = headers(response_)["Content-Type"];
    BOOST_CHECK ( boost::begin(range) != boost::end(range) );
    BOOST_CHECK ( body(response_).size() != 0 );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(http_get_test_different_port, T, tag_types) {
    typedef http::basic_client<T, 1, 1> client_type;
    typename client_type::request request("http://www.boost.org:80/");
    typename client_type::response response_;
    client_type client_;
    BOOST_CHECK_NO_THROW ( response_ = client_.get(request) );
    typename headers_range<typename client_type::response >::type range = headers(response_)["Content-Type"];
    BOOST_CHECK ( boost::begin(range) != boost::end(range) );
    BOOST_CHECK ( body(response_).size() != 0 );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(http_get_test_timeout, T, tag_types) {
    typedef http::basic_client<T, 1, 1> client_type;
    typename client_type::request request("http://localhost:12121/");
    typename client_type::response response_;
    client_type client_;
    BOOST_CHECK_THROW ( response_ = client_.get(request); body(response_); , boost::system::system_error );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(http_get_details, T, tag_types) {
    typedef http::basic_client<T, 1, 1> client_type;
    typename client_type::request request("http://www.boost.org/");
    typename client_type::response response_;
    client_type client_;
    BOOST_CHECK_NO_THROW ( response_ = client_.get(request) );
    BOOST_CHECK_EQUAL ( response_.version().substr(0,7), std::string("HTTP/1.") );
    BOOST_CHECK_EQUAL ( response_.status(), 200u );
    BOOST_CHECK_EQUAL ( response_.status_message(), std::string("OK") );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(http_cached_resolve, T, tag_types) {
    typedef http::basic_client<T, 1, 1> client_type;
    typename client_type::request request("http://www.boost.org/"),
        other_request("http://www.boost.org/users/license.html");
    typename client_type::response response_;
    client_type client_(client_type::cache_resolved);
    BOOST_CHECK_NO_THROW ( response_ = client_.get(request) );
    BOOST_CHECK_NO_THROW ( response_ = client_.get(other_request) );
    response_ = client_.get(other_request);
}

template <class Tag>
struct status_ :
    boost::mpl::if_<
        boost::network::is_async<Tag>,
        boost::mpl::integral_c<boost::uint16_t, 301u>,
        boost::mpl::integral_c<boost::uint16_t, 200u>
    >::type
{};

BOOST_AUTO_TEST_CASE_TEMPLATE(http_redirection_test, T, tag_types) {
    typedef http::basic_client<T, 1, 1> client_type;
    typename client_type::request request("http://boost.org/");
    typename client_type::response response_;
    client_type client_(client_type::follow_redirect);
    BOOST_CHECK_NO_THROW ( response_ = client_.get(request) );
    BOOST_CHECK_EQUAL ( response_.status(), status_<T>::value );
}


