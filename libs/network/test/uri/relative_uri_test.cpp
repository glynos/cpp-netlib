// Copyright (c) Glyn Matthews 2012.
// Copyright 2016 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>
#include <boost/config/warning_disable.hpp>
#include <boost/network/uri/uri.hpp>
#include <boost/network/uri/uri_io.hpp>

using namespace boost::network;

TEST(RelativeURITest, NotSupported) {
  // don't yet support relative URIs
  uri::uri instance("example.com");
  ASSERT_FALSE(uri::valid(instance));
}
