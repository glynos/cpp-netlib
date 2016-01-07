#ifndef CLIENT_TYPES_ROOWQCLE
#define CLIENT_TYPES_ROOWQCLE

// Copyright 2010 Dean Michael Berris.
// Copyright 2015 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>
#include <tuple>
#include "tag_types.hpp"
#include <boost/network/include/http/client.hpp>

// HTTPClientTest is a re-usable fixture for client tests that need to test
// various variations of the client. This is to be used with googletest's
// type-parameteric tests:
//
//   using ClientTypes = ::testing::Types<
//     std::tuple<tag, 1, 0>, std::tuple<tag, 1, 1>>;
//   INSTANTIATE_TYPED_TEST_CASE_P(MyTest, HTTPClientTest, ClientTypes)
//
// We also already provide ClientTypes as a convenience in this header for all
// known and supported client type implementations in the library.
// 
// T must be a tuple in the form: <tag, http version major, http version minor>.
template <class T>
class HTTPClientTest : public ::testing::Test {
 public:
  using ClientType = T;
};

// This is the list of known ClientTypes for testing.
using ClientTypes = ::testing::Types<
    boost::network::http::basic_client<
        boost::network::http::tags::http_default_8bit_tcp_resolve, 1, 0>,
    boost::network::http::basic_client<
        boost::network::http::tags::http_default_8bit_tcp_resolve, 1, 1>,
    boost::network::http::basic_client<
        boost::network::http::tags::http_default_8bit_udp_resolve, 1, 0>,
    boost::network::http::basic_client<
        boost::network::http::tags::http_default_8bit_udp_resolve, 1, 1>>;

#endif /* CLIENT_TYPES_ROOWQCLE */
