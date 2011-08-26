// Copyright 2009, 2010, 2011 Dean Michael Berris, Jeroen Habraken, Glyn Matthews.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt of copy at
// http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_TEST_MODULE URL Test
#include <boost/config/warning_disable.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/network/uri/uri.hpp>
#include <boost/network/uri/accessors.hpp>
#include <boost/network/tags.hpp>
#include <boost/mpl/list.hpp>
#include <boost/range/algorithm/equal.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/filesystem/path.hpp>
#include <map>


using namespace boost::network;


typedef boost::mpl::list<
    tags::default_string
  , tags::default_wstring
    > tag_types;


BOOST_AUTO_TEST_CASE_TEMPLATE(uri_test, T, tag_types) {
    typedef uri::basic_uri<T> uri_type;
    typedef typename uri_type::string_type string_type;

    const std::string url("http://www.example.com/");
    const std::string scheme("http");
    const std::string host("www.example.com");
    const std::string path("/");

    uri_type instance(string_type(boost::begin(url), boost::end(url)));
    BOOST_REQUIRE(uri::is_valid(instance));
    BOOST_CHECK(boost::equal(uri::scheme(instance), scheme));
    BOOST_CHECK(boost::equal(uri::host(instance), host));
    BOOST_CHECK(boost::equal(uri::path(instance), path));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(full_uri_test, T, tag_types) {
    typedef uri::basic_uri<T> uri_type;
    typedef typename uri_type::string_type string_type;

    const std::string url("http://user:password@www.example.com:80/path?query#fragment");
    const std::string scheme("http");
    const std::string user_info("user:password");
    const std::string host("www.example.com");
    const std::string port = "80";
    const std::string path("/path");
    const std::string query("query");
    const std::string fragment("fragment");

    uri_type instance(string_type(boost::begin(url), boost::end(url)));
    BOOST_REQUIRE(uri::is_valid(instance));
    BOOST_CHECK(boost::equal(uri::scheme(instance), scheme));
    BOOST_CHECK(boost::equal(uri::user_info(instance), user_info));
    BOOST_CHECK(boost::equal(uri::host(instance), host));
    BOOST_CHECK_EQUAL(static_cast<unsigned short>(uri::port_us(instance)), 80);
    BOOST_CHECK(boost::equal(uri::port(instance), port));
    BOOST_CHECK(boost::equal(uri::path(instance), path));
    BOOST_CHECK(boost::equal(uri::query(instance), query));
    BOOST_CHECK(boost::equal(uri::fragment(instance), fragment));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(mailto_test, T, tag_types) {
    typedef uri::basic_uri<T> uri_type;
    typedef typename uri_type::string_type string_type;

    const std::string url("mailto:john.doe@example.com");
    const std::string scheme("mailto");
    // RFC 3986 interprets this as the path
    const std::string path("john.doe@example.com");

    uri_type instance(string_type(boost::begin(url), boost::end(url)));
    BOOST_REQUIRE(uri::is_valid(instance));
    BOOST_CHECK(boost::equal(uri::scheme(instance), scheme));
    BOOST_CHECK(boost::equal(uri::path(instance), path));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(file_test, T, tag_types) {
    typedef uri::basic_uri<T> uri_type;
    typedef typename uri_type::string_type string_type;

    const std::string url("file:///bin/bash");
    const std::string scheme("file");
    const std::string path("/bin/bash");

    uri_type instance(string_type(boost::begin(url), boost::end(url)));
    BOOST_REQUIRE(uri::is_valid(instance));
    BOOST_CHECK(boost::equal(uri::scheme(instance), scheme));
    BOOST_CHECK(boost::equal(uri::path(instance), path));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(xmpp_test, T, tag_types) {
    typedef uri::basic_uri<T> uri_type;
    typedef typename uri_type::string_type string_type;

    const std::string url("xmpp:example-node@example.com?message;subject=Hello%20World");
    const std::string scheme("xmpp");
    // RFC 3986 interprets this as the path
    const std::string path("example-node@example.com");
    const std::string query("message;subject=Hello%20World");

    uri_type instance(string_type(boost::begin(url), boost::end(url)));
    BOOST_REQUIRE(uri::is_valid(instance));
    BOOST_CHECK(boost::equal(uri::scheme(instance), scheme));
    BOOST_CHECK(boost::equal(uri::path(instance), path));
    BOOST_CHECK(boost::equal(uri::query(instance), query));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(ipv4_address_test, T, tag_types) {
    typedef uri::basic_uri<T> uri_type;
    typedef typename uri_type::string_type string_type;

    const std::string url("http://129.79.245.252/");
    const std::string scheme("http");
    const std::string host("129.79.245.252");
    const std::string path("/");

    uri_type instance(string_type(boost::begin(url), boost::end(url)));
    BOOST_REQUIRE(uri::is_valid(instance));
    BOOST_CHECK(boost::equal(uri::scheme(instance), scheme));
    BOOST_CHECK(boost::equal(uri::host(instance), host));
    BOOST_CHECK(boost::equal(uri::path(instance), path));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(ipv6_address_test_1, T, tag_types) {
    typedef uri::basic_uri<T> uri_type;
    typedef typename uri_type::string_type string_type;

    const std::string url("http://[1080:0:0:0:8:800:200C:417A]/");
    const std::string scheme("http");
    const std::string host("[1080:0:0:0:8:800:200C:417A]");
    const std::string path("/");

    uri_type instance(string_type(boost::begin(url), boost::end(url)));
    BOOST_REQUIRE(uri::is_valid(instance));
    BOOST_CHECK(boost::equal(uri::scheme(instance), scheme));
    BOOST_CHECK(boost::equal(uri::host(instance), host));
    BOOST_CHECK(boost::equal(uri::path(instance), path));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(ipv6_address_test_2, T, tag_types) {
    typedef uri::basic_uri<T> uri_type;
    typedef typename uri_type::string_type string_type;

    const std::string url("http://[2001:db8:85a3:8d3:1319:8a2e:370:7348]/");
    const std::string scheme("http");
    const std::string host("[2001:db8:85a3:8d3:1319:8a2e:370:7348]");
    const std::string path("/");

    uri_type instance(string_type(boost::begin(url), boost::end(url)));
    BOOST_REQUIRE(uri::is_valid(instance));
    BOOST_CHECK(boost::equal(uri::scheme(instance), scheme));
    BOOST_CHECK(boost::equal(uri::host(instance), host));
    BOOST_CHECK(boost::equal(uri::path(instance), path));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(ftp_test, T, tag_types) {
    typedef uri::basic_uri<T> uri_type;
    typedef typename uri_type::string_type string_type;

    const std::string url("ftp://john.doe@ftp.example.com/");
    const std::string scheme("ftp");
    const std::string user_info("john.doe");
    const std::string host("ftp.example.com");
    const std::string path("/");

    uri_type instance(string_type(boost::begin(url), boost::end(url)));
    BOOST_REQUIRE(uri::is_valid(instance));
    BOOST_CHECK(boost::equal(uri::scheme(instance), scheme));
    BOOST_CHECK(boost::equal(uri::user_info(instance), user_info));
    BOOST_CHECK(boost::equal(uri::host(instance), host));
    BOOST_CHECK(boost::equal(uri::path(instance), path));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(news_test, T, tag_types) {
    typedef uri::basic_uri<T> uri_type;
    typedef typename uri_type::string_type string_type;

    const std::string url("news:comp.infosystems.www.servers.unix");
    const std::string scheme("news");
    const std::string path("comp.infosystems.www.servers.unix");

    uri_type instance(string_type(boost::begin(url), boost::end(url)));
    BOOST_REQUIRE(uri::is_valid(instance));
    BOOST_CHECK(boost::equal(uri::scheme(instance), scheme));
    BOOST_CHECK(boost::equal(uri::path(instance), path));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(tel_test, T, tag_types) {
    typedef uri::basic_uri<T> uri_type;
    typedef typename uri_type::string_type string_type;

    const std::string url("tel:+1-816-555-1212");
    const std::string scheme("tel");
    const std::string path("+1-816-555-1212");

    uri_type instance(string_type(boost::begin(url), boost::end(url)));
    BOOST_REQUIRE(uri::is_valid(instance));
    BOOST_CHECK(boost::equal(uri::scheme(instance), scheme));
    BOOST_CHECK(boost::equal(uri::path(instance), path));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(encoded_uri_test, T, tag_types) {
    typedef uri::basic_uri<T> uri_type;
    typedef typename uri_type::string_type string_type;

    const std::string url("http://www.example.com/Path%20With%20%28Some%29%20Encoded%20Characters%21");
    const std::string scheme("http");
    const std::string host("www.example.com");
    const std::string encoded_path("/Path%20With%20%28Some%29%20Encoded%20Characters%21");
    const std::string decoded_path("/Path With (Some) Encoded Characters!");

    uri_type instance(string_type(boost::begin(url), boost::end(url)));
    BOOST_REQUIRE(uri::is_valid(instance));
    BOOST_CHECK(boost::equal(uri::scheme(instance), scheme));
    BOOST_CHECK(boost::equal(uri::host(instance), host));
    BOOST_CHECK(boost::equal(uri::path(instance), encoded_path));
    BOOST_CHECK(boost::equal(uri::decoded_path(instance), decoded_path));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(copy_constructor_test, T, tag_types) {
    typedef uri::basic_uri<T> uri_type;
    typedef typename uri_type::string_type string_type;

    const std::string url("http://www.example.com/");

    uri_type instance(string_type(boost::begin(url), boost::end(url)));
    uri_type copy = instance;
    BOOST_CHECK(instance == copy);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(assignment_test, T, tag_types) {
    typedef uri::basic_uri<T> uri_type;
    typedef typename uri_type::string_type string_type;

    const std::string url("http://www.example.com/");

    uri_type instance(string_type(boost::begin(url), boost::end(url)));
    uri_type copy;
    copy = instance;
    BOOST_CHECK(instance.string() == copy.string());
    BOOST_CHECK(instance == copy);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(username_test, T, tag_types) {
    typedef uri::basic_uri<T> uri_type;
    typedef typename uri_type::string_type string_type;

    const std::string url("ftp://john.doe:password@ftp.example.com/");
    const std::string scheme("http");
    const std::string username("john.doe");
    const std::string password("password");

    uri_type instance(string_type(boost::begin(url), boost::end(url)));
    BOOST_REQUIRE(uri::is_valid(instance));
    BOOST_CHECK(boost::equal(uri::username(instance), username));
    BOOST_CHECK(boost::equal(uri::password(instance), password));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(authority_test, T, tag_types) {
    typedef uri::basic_uri<T> uri_type;
    typedef typename uri_type::string_type string_type;

    const std::string url("http://user:password@www.example.com:80/path?query#fragment");
    const std::string authority("user:password@www.example.com:80");

    uri_type instance(string_type(boost::begin(url), boost::end(url)));
    BOOST_REQUIRE(uri::is_valid(instance));
    BOOST_CHECK(boost::equal(uri::authority(instance), authority));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(http_query_map_test, T, tag_types) {
    typedef uri::basic_uri<T> uri_type;
    typedef typename uri_type::string_type string_type;

    const std::string url("http://user:password@www.example.com:80/path?query#fragment");

    uri_type instance(string_type(boost::begin(url), boost::end(url)));
    BOOST_REQUIRE(uri::is_valid(instance));
    const std::string key("query");
    const std::string value;

    std::map<string_type, string_type> queries;
    uri::query_map(instance, queries);
    BOOST_REQUIRE_EQUAL(queries.size(), std::size_t(1));
    BOOST_CHECK(boost::equal(queries.begin()->first, key));
    BOOST_CHECK(boost::equal(queries.begin()->second, value));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(xmpp_query_map_test, T, tag_types) {
    typedef uri::basic_uri<T> uri_type;
    typedef typename uri_type::string_type string_type;

    const std::string url("xmpp:example-node@example.com?message;subject=Hello%20World");

    uri_type instance(string_type(boost::begin(url), boost::end(url)));
    BOOST_REQUIRE(uri::is_valid(instance));
    const std::string key_1("message"), key_2("subject");
    const std::string value_1, value_2("Hello%20World");

    std::map<string_type, string_type> queries;
    uri::query_map(instance, queries);
    BOOST_REQUIRE_EQUAL(queries.size(), std::size_t(2));
    BOOST_CHECK(boost::equal(queries.begin()->first, key_1));
    BOOST_CHECK(boost::equal(queries.begin()->second, value_1));
    BOOST_CHECK(boost::equal((++queries.begin())->first, key_2));
    BOOST_CHECK(boost::equal((++queries.begin())->second, value_2));
}
