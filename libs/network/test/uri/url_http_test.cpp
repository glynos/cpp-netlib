//            Copyright (c) Glyn Matthews 2011.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#define BOOST_TEST_MODULE HTTP URL Test
#include <boost/config/warning_disable.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/network/uri/http/uri.hpp>
#include <boost/mpl/list.hpp>
#include <boost/range/algorithm/equal.hpp>

using namespace boost::network;

typedef boost::mpl::list<
    tags::default_string
  , tags::default_wstring
    > tag_types;


BOOST_AUTO_TEST_CASE_TEMPLATE(valid_http, T, tag_types)
{
    typedef uri::http::basic_uri<T> uri_type;
    typedef typename uri_type::string_type string_type;
    const std::string url("http://example.com/");
    uri_type instance(string_type(boost::begin(url), boost::end(url)));
    BOOST_REQUIRE(uri::is_http(instance));
    BOOST_REQUIRE(uri::is_valid(instance));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(valid_https, T, tag_types)
{
    typedef uri::http::basic_uri<T> uri_type;
    typedef typename uri_type::string_type string_type;
    const std::string url("https://example.com/");
    uri_type instance(string_type(boost::begin(url), boost::end(url)));
    BOOST_REQUIRE(uri::is_https(instance));
    BOOST_REQUIRE(uri::is_valid(instance));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(invalid_https, T, tag_types)
{
    typedef uri::http::basic_uri<T> uri_type;
    typedef typename uri_type::string_type string_type;
    const std::string url("http://example.com/");
    uri_type instance(string_type(boost::begin(url), boost::end(url)));
    BOOST_REQUIRE(!uri::is_https(instance));
    BOOST_REQUIRE(uri::is_valid(instance));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(not_http, T, tag_types)
{
    typedef uri::http::basic_uri<T> uri_type;
    typedef typename uri_type::string_type string_type;
    const std::string url("mailto:john.doe@example.com");
    uri_type instance(string_type(boost::begin(url), boost::end(url)));
    BOOST_REQUIRE(!uri::is_http(instance));
    BOOST_REQUIRE(!uri::is_https(instance));
    BOOST_REQUIRE(!uri::is_valid(instance));
}
