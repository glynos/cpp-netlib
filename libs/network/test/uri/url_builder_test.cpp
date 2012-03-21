//            Copyright (c) Glyn Matthews 2012.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_TEST_MODULE URI builder test
#include <boost/config/warning_disable.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/network/uri/uri.hpp>
#include <boost/network/uri/directives.hpp>
#include <boost/network/uri/accessors.hpp>
#include <boost/network/uri/uri_io.hpp>
#include <boost/network/tags.hpp>
#include <boost/mpl/list.hpp>
#include <boost/range/algorithm/equal.hpp>
#include <boost/range/algorithm/copy.hpp>


using namespace boost::network;


BOOST_AUTO_TEST_CASE(builder_test)
{
    uri::uri instance;
    uri::builder builder(instance);
    builder
        .scheme("http")
        .host("www.example.com")
        .path("/")
        ;
    BOOST_REQUIRE(uri::valid(instance));
    BOOST_CHECK_EQUAL("http://www.example.com/", instance.string());
    BOOST_CHECK_EQUAL(uri::scheme(instance), "http");
    BOOST_CHECK_EQUAL(uri::host(instance), "www.example.com");
    BOOST_CHECK_EQUAL(uri::path(instance), "/");
}

BOOST_AUTO_TEST_CASE(full_uri_builder_test)
{
    uri::uri instance;
    uri::builder builder(instance);
    builder
        .scheme("http")
        .user_info("user:password")
        .host("www.example.com")
        .port("80")
        .path("/path")
        .query("query")
        .fragment("fragment")
        ;
    BOOST_REQUIRE(uri::valid(instance));
    BOOST_CHECK_EQUAL(uri::scheme(instance), "http");
    BOOST_CHECK_EQUAL(uri::user_info(instance), "user:password");
    BOOST_CHECK_EQUAL(uri::host(instance), "www.example.com");
    BOOST_CHECK(uri::port_us(instance));
    BOOST_CHECK_EQUAL(uri::port_us(instance).get(), 80);
    BOOST_CHECK_EQUAL(uri::port(instance), "80");
    BOOST_CHECK_EQUAL(uri::path(instance), "/path");
    BOOST_CHECK_EQUAL(uri::query(instance), "query");
    BOOST_CHECK_EQUAL(uri::fragment(instance), "fragment");
}

BOOST_AUTO_TEST_CASE(port_test)
{
    uri::uri instance;
    uri::builder(instance).scheme("http").host("www.example.com").port(8000).path("/");
    BOOST_REQUIRE(uri::valid(instance));
    BOOST_CHECK_EQUAL(uri::scheme(instance), "http");
    BOOST_CHECK_EQUAL(uri::host(instance), "www.example.com");
    BOOST_CHECK_EQUAL(uri::port(instance), "8000");
    BOOST_CHECK_EQUAL(uri::path(instance), "/");
}

BOOST_AUTO_TEST_CASE(encoded_path_test)
{
    uri::uri instance;
    uri::builder builder(instance);
    builder
        .scheme("http")
        .host("www.example.com")
        .port(8000)
        .encoded_path("/Path With (Some) Encoded Characters!")
        ;
    BOOST_REQUIRE(uri::valid(instance));
    BOOST_CHECK_EQUAL(uri::scheme(instance), "http");
    BOOST_CHECK_EQUAL(uri::host(instance), "www.example.com");
    BOOST_CHECK_EQUAL(uri::port(instance), "8000");
    BOOST_CHECK_EQUAL(uri::path(instance), "/Path%20With%20%28Some%29%20Encoded%20Characters%21");
    BOOST_CHECK_EQUAL(uri::decoded_path(instance), "/Path With (Some) Encoded Characters!");
}

BOOST_AUTO_TEST_CASE(query_test)
{
    uri::uri instance;
    uri::builder builder(instance);
    builder
       .scheme("http")
       .host("www.example.com")
       .path("/")
       .query("key", "value")
        ;
    BOOST_REQUIRE(uri::valid(instance));
    BOOST_CHECK_EQUAL(uri::scheme(instance), "http");
    BOOST_CHECK_EQUAL(uri::host(instance), "www.example.com");
    BOOST_CHECK_EQUAL(uri::path(instance), "/");
    BOOST_CHECK_EQUAL(uri::query(instance), "key=value");
}

BOOST_AUTO_TEST_CASE(query_2_test)
{
    uri::uri instance;
    uri::builder builder(instance);
    builder
       .scheme("http")
       .host("www.example.com")
       .path("/")
       .query("key1", "value1")
       .query("key2", "value2")
        ;
    BOOST_REQUIRE(uri::valid(instance));
    BOOST_CHECK_EQUAL(uri::scheme(instance), "http");
    BOOST_CHECK_EQUAL(uri::host(instance), "www.example.com");
    BOOST_CHECK_EQUAL(uri::path(instance), "/");
    BOOST_CHECK_EQUAL(uri::query(instance), "key1=value1&key2=value2");
}

BOOST_AUTO_TEST_CASE(fragment_test)
{
    uri::uri instance;
    uri::builder builder(instance);
    builder
       .scheme("http")
       .host("www.example.com")
       .path("/")
       .fragment("fragment")
        ;
    BOOST_REQUIRE(uri::valid(instance));
    BOOST_CHECK_EQUAL(uri::scheme(instance), "http");
    BOOST_CHECK_EQUAL(uri::host(instance), "www.example.com");
    BOOST_CHECK_EQUAL(uri::path(instance), "/");
    BOOST_CHECK_EQUAL(uri::fragment(instance), "fragment");
}

BOOST_AUTO_TEST_CASE(from_base_test)
{
    uri::uri base_uri("http://www.example.com");
    uri::uri instance(base_uri);
    uri::builder builder(instance);
    builder
        .path("/")
        .fragment("fragment")
        ;
    BOOST_REQUIRE(uri::valid(instance));
    BOOST_CHECK_EQUAL(uri::scheme(instance), "http");
    BOOST_CHECK_EQUAL(uri::host(instance), "www.example.com");
    BOOST_CHECK_EQUAL(uri::path(instance), "/");
    BOOST_CHECK_EQUAL(uri::fragment(instance), "fragment");
}

BOOST_AUTO_TEST_CASE(encoded_null_char_test)
{
    // there is a potential bug in the way we process ranges if the
    // strings are null terminated.
    uri::uri instance;
    uri::builder builder(instance);
    builder
       .scheme("http")
       .host("www.example.com")
       .encoded_path("/")
        ;
    BOOST_REQUIRE(uri::valid(instance));
    BOOST_CHECK_EQUAL(uri::scheme(instance), "http");
    BOOST_CHECK_EQUAL(uri::host(instance), "www.example.com");
    BOOST_CHECK_EQUAL(uri::path(instance), "/");
}

BOOST_AUTO_TEST_CASE(mailto_builder_test)
{
    uri::uri instance;
    uri::builder builder(instance);
    builder
       .scheme("mailto")
       .path("cpp-netlib@example.com")
        ;
    BOOST_REQUIRE(uri::valid(instance));
    BOOST_CHECK_EQUAL(uri::scheme(instance), "mailto");
    BOOST_CHECK_EQUAL(uri::path(instance), "cpp-netlib@example.com");
}
