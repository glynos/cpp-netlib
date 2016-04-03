// Copyright 2016 Glyn Matthews.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>
#include <boost/network/protocol/http/client/client_request.hpp>

namespace http = boost::network::http;

TEST(http_client_request_test, default_construct)
{
  ASSERT_NO_THROW(http::client_request{});
}
