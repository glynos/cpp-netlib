//            Copyright (c) Glyn Matthews 2011.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_TEST_MODULE URI builder test
#include <boost/config/warning_disable.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/network/uri/uri.hpp>
#include <boost/network/uri/builder.hpp>
#include <boost/network/tags.hpp>
#include <boost/mpl/list.hpp>
#include <boost/range/algorithm/equal.hpp>
#include <boost/range/algorithm/copy.hpp>


using namespace boost::network;

typedef boost::mpl::list<
    tags::default_string
//  , tags::default_wstring
    > tag_types;


BOOST_AUTO_TEST_CASE_TEMPLATE(builder_test, T, tag_types)
{
    typedef uri::basic_uri<T> uri_type;
    typedef typename uri_type::string_type string_type;

    const std::string scheme("http");
    const std::string host("www.example.com");
    const std::string path("/");

    uri_type instance;
    uri::basic_builder<T> builder(instance);
    builder << uri::builder::scheme(string_type(boost::begin(scheme),
                                                boost::end(scheme)))
            << uri::builder::host(string_type(boost::begin(host),
                                              boost::end(host)))
            << uri::builder::path(string_type(boost::begin(path),
                                              boost::end(path)))
        ;
    BOOST_REQUIRE(uri::is_valid(instance));
    BOOST_CHECK(boost::equal(uri::scheme(instance), scheme));
    BOOST_CHECK(boost::equal(uri::host(instance), host));
    BOOST_CHECK(boost::equal(uri::path(instance), path));
}
