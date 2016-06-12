
// Copyright 2010 Dean Michael Berris.
// Copyright 2016 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <memory>
#include <gtest/gtest.h>
#include <boost/network/include/http/server.hpp>

namespace http = boost::network::http;
namespace util = boost::network::utils;

struct dummy_async_handler;
typedef http::server<dummy_async_handler> async_server;

struct dummy_async_handler {
  void operator()(async_server::request const &, async_server::connection_ptr) {
    // Really, this is just for testing purposes
  }
};

TEST(HTTPServerTest, MinimalConstructor) {
  dummy_async_handler async_handler;
  async_server::options async_options(async_handler);
  ASSERT_NO_THROW(async_server async_instance(
      async_options.address("127.0.0.1").port("80")));
}

TEST(HTTPServerTest, WithIOServiceParameter) {
  dummy_async_handler async_handler;
  std::shared_ptr<util::thread_pool> thread_pool;
  std::shared_ptr<boost::asio::io_service> io_service;
  async_server::options async_options(async_handler);

  ASSERT_NO_THROW(async_server async_instance(async_options.address("127.0.0.1")
                                                  .port("80")
                                                  .io_service(io_service)
                                                  .thread_pool(thread_pool)));
}

TEST(HTTPServerTes, ThrowsOnFailure) {
  dummy_async_handler async_handler;
  std::shared_ptr<util::thread_pool> thread_pool;
  std::shared_ptr<boost::asio::io_service> io_service;
  async_server::options async_options(async_handler);
  async_server async_instance(async_options.address("127.0.0.1")
                                  .port("80")
                                  .io_service(io_service)
                                  .thread_pool(thread_pool));
  EXPECT_THROW(async_instance.run(), std::runtime_error);
}
