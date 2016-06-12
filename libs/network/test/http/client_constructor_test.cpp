
// Copyright 2010 Dean Michael Berris.
// Copyright 2015 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// Migrated from using Boost.Test to using googletest intead.
#include <gtest/gtest.h>

#include <boost/network/include/http/client.hpp>
#include "client_types.hpp"

namespace http = boost::network::http;

TYPED_TEST_CASE(HTTPClientTest, ClientTypes);

TYPED_TEST(HTTPClientTest, Constructors) {
  typename TypeParam::options options;
  TypeParam instance;
  TypeParam instance2(
      options.io_service(std::make_shared<boost::asio::io_service>()));
}

TYPED_TEST(HTTPClientTest, ConstructorsWithOptions) {
  typename TypeParam::options options;
  TypeParam instance(options.follow_redirects(true).cache_resolved(true));
  TypeParam instance2(
      options.openssl_certificate("foo").openssl_verify_path("bar"));
  TypeParam instance3(
      options.openssl_certificate_file("foo").openssl_private_key_file("bar"));
  TypeParam instance4(
      options.follow_redirects(true)
          .io_service(std::make_shared<boost::asio::io_service>())
          .cache_resolved(true));
}
