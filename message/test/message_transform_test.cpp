
//          Copyright Dean Michael Berris 2007.
// Copyright 2012 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifdef BUILD_SHARED_LIBS
# define BOOST_TEST_DYN_LINK
#endif
#define BOOST_TEST_MODULE message test
#include <boost/config/warning_disable.hpp>
#include <boost/test/unit_test.hpp>
#include <network/message.hpp>
#include <algorithm>

BOOST_AUTO_TEST_CASE ( message_transform_toupper ) {
    using namespace network;

    message msg;
    msg << source("me");
    std::string const & source_orig = source(msg);
    BOOST_CHECK_EQUAL ( source_orig, "me" );
    msg << transform(to_upper_, source_);
    std::string const & source_upper = source(msg);
    BOOST_CHECK_EQUAL ( source_upper, "ME" );
    msg << destination("you");
    std::string const & destination_orig = destination(msg);
    BOOST_CHECK_EQUAL ( destination_orig, "you");
    msg << transform(to_upper_, destination_);
    std::string const & destination_upper = destination(msg);
    BOOST_CHECK_EQUAL ( destination_upper, "YOU");
}

BOOST_AUTO_TEST_CASE ( message_transform_tolower ) {
    using namespace network;

    message msg;
    msg << source("ME");
    std::string const & source_orig = source(msg);
    BOOST_CHECK_EQUAL ( source_orig, "ME" );
    msg << transform(to_lower_, source_);
    std::string const & source_lower = source(msg);
    BOOST_CHECK_EQUAL ( source_lower, "me" );
    msg << destination("YOU");
    std::string const & destination_orig = destination(msg);
    BOOST_CHECK_EQUAL ( destination_orig, "YOU" );
    msg << transform(to_lower_, destination_);
    std::string const & destination_lower = destination(msg);
    BOOST_CHECK_EQUAL ( destination_lower, "you" );
}

