
// Copyright 2010 Dean Michael Berris.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_TEST_MODULE HTTP Server Construtor Tests

#include <boost/network/include/http/server.hpp>
#include <boost/test/unit_test.hpp>

namespace http = boost::network::http;
namespace util = boost::network::utils;

struct dummy_async_handler;
typedef http::server<dummy_async_handler> async_server;

struct dummy_async_handler {
  void operator()(async_server::request const &, async_server::connection_ptr) {
    // Really, this is just for testing purposes
  }
};

BOOST_AUTO_TEST_CASE(minimal_constructor) {
  dummy_async_handler async_handler;
  async_server::options async_options(async_handler);
  BOOST_CHECK_NO_THROW(async_server async_instance(
      async_options.address("127.0.0.1").port("80")));
}

BOOST_AUTO_TEST_CASE(with_io_service_parameter) {
  dummy_async_handler async_handler;
  boost::shared_ptr<util::thread_pool> thread_pool;
  boost::shared_ptr<boost::asio::io_service> io_service;
  async_server::options async_options(async_handler);

  BOOST_CHECK_NO_THROW(
      async_server async_instance(async_options.address("127.0.0.1")
                                      .port("80")
                                      .io_service(io_service)
                                      .thread_pool(thread_pool)));
}

BOOST_AUTO_TEST_CASE(throws_on_failure) {
  dummy_async_handler async_handler;
  boost::shared_ptr<util::thread_pool> thread_pool;
  boost::shared_ptr<boost::asio::io_service> io_service;
  async_server::options async_options(async_handler);
  async_server async_instance(async_options.address("127.0.0.1")
                                  .port("80")
                                  .io_service(io_service)
                                  .thread_pool(thread_pool));
  BOOST_CHECK_THROW(async_instance.run(), std::runtime_error);
}
