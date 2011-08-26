//            Copyright (c) Glyn Matthews 2011.
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

typedef boost::mpl::list<
    tags::default_string
  , tags::default_wstring
    > tag_types;


BOOST_AUTO_TEST_CASE_TEMPLATE(builder_test, T, tag_types)
{
    typedef uri::basic_uri<T> uri_type;
    typedef typename uri_type::string_type string_type;

    const std::string scheme("http");
    const std::string host("www.example.com");
    const std::string path("/");

    uri_type instance;
    //instance << uri::scheme("http") << uri::host("www.example.com") << uri::path("/");
    instance << uri::scheme(string_type(boost::begin(scheme), boost::end(scheme)))
             << uri::host(string_type(boost::begin(host), boost::end(host)))
             << uri::path(string_type(boost::begin(path), boost::end(path)))
        ;
    BOOST_REQUIRE(uri::is_valid(instance));
    BOOST_CHECK(boost::equal(uri::scheme(instance), scheme));
    BOOST_CHECK(boost::equal(uri::host(instance), host));
    BOOST_CHECK(boost::equal(uri::path(instance), path));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(full_uri_builder_test, T, tag_types)
{
    typedef uri::basic_uri<T> uri_type;
    typedef typename uri_type::string_type string_type;

    const std::string scheme("http");
    const std::string user_info("user:password");
    const std::string host("www.example.com");
    const std::string port("80");
    const std::string path("/path");
    const std::string query("query");
    const std::string fragment("fragment");

    uri_type instance;
    instance << uri::scheme(string_type(boost::begin(scheme), boost::end(scheme)))
             << uri::user_info(string_type(boost::begin(user_info), boost::end(user_info)))
             << uri::host(string_type(boost::begin(host), boost::end(host)))
             << uri::port(string_type(boost::begin(port), boost::end(port)))
             << uri::path(string_type(boost::begin(path), boost::end(path)))
             << uri::query(string_type(boost::begin(query), boost::end(query)))
             << uri::fragment(string_type(boost::begin(fragment), boost::end(fragment)))
        ;
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

BOOST_AUTO_TEST_CASE_TEMPLATE(port_test, T, tag_types)
{
    typedef uri::basic_uri<T> uri_type;
    typedef typename uri_type::string_type string_type;

    const std::string scheme("http");
    const std::string host("www.example.com");
    const std::string port("8000");
    const std::string path("/");

    uri_type instance;
    //instance << uri::scheme("http") << uri::host("www.example.com") << uri::port(8000) << uri::path("/");
    instance << uri::scheme(string_type(boost::begin(scheme), boost::end(scheme)))
             << uri::host(string_type(boost::begin(host), boost::end(host)))
             << uri::port(8000)
             << uri::path(string_type(boost::begin(path), boost::end(path)))
        ;
    BOOST_REQUIRE(uri::is_valid(instance));
    BOOST_CHECK(boost::equal(uri::scheme(instance), scheme));
    BOOST_CHECK(boost::equal(uri::host(instance), host));
    BOOST_CHECK(boost::equal(uri::port(instance), port));
    BOOST_CHECK(boost::equal(uri::path(instance), path));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(encoded_path_test, T, tag_types)
{
    typedef uri::basic_uri<T> uri_type;
    typedef typename uri_type::string_type string_type;

    const std::string scheme("http");
    const std::string host("www.example.com");
    const std::string port("8000");
    const std::string encoded_path("/Path%20With%20%28Some%29%20Encoded%20Characters%21");
    const std::string decoded_path("/Path With (Some) Encoded Characters!");

    uri_type instance;
    instance << uri::scheme(string_type(boost::begin(scheme), boost::end(scheme)))
             << uri::host(string_type(boost::begin(host), boost::end(host)))
             << uri::port(8000)
             << uri::path(string_type(boost::begin(decoded_path), boost::end(decoded_path)))
        ;
    BOOST_REQUIRE(uri::is_valid(instance));
    BOOST_CHECK(boost::equal(uri::scheme(instance), scheme));
    BOOST_CHECK(boost::equal(uri::host(instance), host));
    BOOST_CHECK(boost::equal(uri::port(instance), port));
    BOOST_CHECK(boost::equal(uri::path(instance), encoded_path));
    BOOST_CHECK(boost::equal(uri::decoded_path(instance), decoded_path));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(query_test, T, tag_types)
{
    typedef uri::basic_uri<T> uri_type;
    typedef typename uri_type::string_type string_type;

    const std::string scheme("http");
    const std::string host("www.example.com");
    const std::string path("/");
    const std::string query("key=value");
    const std::string query_key("key");
    const std::string query_value("value");

    uri_type instance;
    //instance << uri::scheme("http") << uri::host("www.example.com") << uri::path("/");
    instance << uri::scheme(string_type(boost::begin(scheme), boost::end(scheme)))
             << uri::host(string_type(boost::begin(host), boost::end(host)))
             << uri::path(string_type(boost::begin(path), boost::end(path)))
             << uri::query(string_type(boost::begin(query_key), boost::end(query_key)),
                           string_type(boost::begin(query_value), boost::end(query_value)));
    BOOST_REQUIRE(uri::is_valid(instance));
    BOOST_CHECK(boost::equal(uri::scheme(instance), scheme));
    BOOST_CHECK(boost::equal(uri::host(instance), host));
    BOOST_CHECK(boost::equal(uri::path(instance), path));
    BOOST_CHECK(boost::equal(uri::query(instance), query));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(query_2_test, T, tag_types)
{
    typedef uri::basic_uri<T> uri_type;
    typedef typename uri_type::string_type string_type;

    const std::string scheme("http");
    const std::string host("www.example.com");
    const std::string path("/");
    const std::string query("key=value;key=value");
    const std::string query_key("key");
    const std::string query_value("value");

    uri_type instance;
    //instance << uri::scheme("http") << uri::host("www.example.com") << uri::path("/");
    instance << uri::scheme(string_type(boost::begin(scheme), boost::end(scheme)))
             << uri::host(string_type(boost::begin(host), boost::end(host)))
             << uri::path(string_type(boost::begin(path), boost::end(path)))
             << uri::query(string_type(boost::begin(query_key), boost::end(query_key)),
                           string_type(boost::begin(query_value), boost::end(query_value)))
             << uri::query(string_type(boost::begin(query_key), boost::end(query_key)),
                           string_type(boost::begin(query_value), boost::end(query_value)));
    BOOST_REQUIRE(uri::is_valid(instance));
    BOOST_CHECK(boost::equal(uri::scheme(instance), scheme));
    BOOST_CHECK(boost::equal(uri::host(instance), host));
    BOOST_CHECK(boost::equal(uri::path(instance), path));
    BOOST_CHECK(boost::equal(uri::query(instance), query));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(fragment_test, T, tag_types)
{
    typedef uri::basic_uri<T> uri_type;
    typedef typename uri_type::string_type string_type;

    const std::string scheme("http");
    const std::string host("www.example.com");
    const std::string path("/");
    const std::string fragment("fragment");

    uri_type instance;
    //instance << uri::scheme("http") << uri::host("www.example.com") << uri::path("/") << uri::fragment("fragment");
    instance << uri::scheme(string_type(boost::begin(scheme), boost::end(scheme)))
             << uri::host(string_type(boost::begin(host), boost::end(host)))
             << uri::path(string_type(boost::begin(path), boost::end(path)))
             << uri::fragment(string_type(boost::begin(fragment), boost::end(fragment)));
    BOOST_REQUIRE(uri::is_valid(instance));
    BOOST_CHECK(boost::equal(uri::scheme(instance), scheme));
    BOOST_CHECK(boost::equal(uri::host(instance), host));
    BOOST_CHECK(boost::equal(uri::path(instance), path));
    BOOST_CHECK(boost::equal(uri::fragment(instance), fragment));
}
