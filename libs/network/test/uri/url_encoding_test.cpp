//            Copyright (c) Glyn Matthews 2011.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_TEST_MODULE URL encoding test
#include <boost/config/warning_disable.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/network/uri/encode.hpp>
#include <boost/network/uri/decode.hpp>
#include <boost/network/tags.hpp>
#include <boost/network/traits/string.hpp>
#include <boost/mpl/list.hpp>
#include <boost/range/algorithm/equal.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <iterator>


using namespace boost::network;

typedef boost::mpl::list<
    tags::default_string
  , tags::default_wstring
    > tag_types;


BOOST_AUTO_TEST_CASE_TEMPLATE(encoding_test, T, tag_types) {
    typedef typename string<T>::type string_type;

    const std::string unencoded(" !\"#$%&\'()*");
    const std::string encoded("%20%21%22%23%24%25%26%27%28%29%2A");

    string_type instance;
    uri::encode(unencoded, std::back_inserter(instance));
    BOOST_CHECK(boost::equal(instance, encoded));
}


BOOST_AUTO_TEST_CASE_TEMPLATE(decoding_test, T, tag_types) {
    typedef typename string<T>::type string_type;

    const std::string unencoded(" !\"#$%&\'()*");
    const std::string encoded("%20%21%22%23%24%25%26%27%28%29%2A");

    string_type instance;
    uri::decode(encoded, std::back_inserter(instance));
    BOOST_CHECK(boost::equal(instance, unencoded));
}
