#ifdef BUILD_SHARED_LIBS
# define BOOST_TEST_DYN_LINK
# define BOOST_TEST_NO_MAIN
#endif
#define BOOST_TEST_MODULE HTTP Asynchronous Server Tests

#include <network/include/http/server.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/thread.hpp>

namespace http = network::http;
namespace util = network::utils;

struct dummy_async_handler;
typedef http::async_server<dummy_async_handler> async_server;

struct dummy_async_handler {
    void operator()(async_server::request const & req,
                    async_server::connection_ptr conn) {
        // Really, this is just for testing purposes
    }
};

// In this batch of tests we ensure that calling run and stop on an async_server, in any sequence, is thread safe.

int main(int argc, char * argv[]) {
    dummy_async_handler async_handler;
    http::server_options options;
    options.address("127.0.0.1")
           .port("8007")
           .reuse_address(true);

#define ASYNC_SERVER_TEST_CONFIG  \
  options, async_handler, pool

#define ASYNC_SERVER_SLEEP_TIME    \
  boost::posix_time::milliseconds(100)

  // stop from main thread
  {
    NETWORK_MESSAGE("TEST: stop without running");
    util::thread_pool pool;
    async_server server_instance(ASYNC_SERVER_TEST_CONFIG);
    server_instance.stop();
  }

  // run-stop from main thread
  {
    NETWORK_MESSAGE("TEST: stop from main thread");
    util::thread_pool pool;
    async_server server_instance(ASYNC_SERVER_TEST_CONFIG);

    boost::thread running_thread(boost::bind(&async_server::run, &server_instance));
    boost::this_thread::sleep(ASYNC_SERVER_SLEEP_TIME);

    server_instance.stop();
    running_thread.join();
  }

  // run-stop from another thread
  {
    NETWORK_MESSAGE("TEST: stop from another thread");
    util::thread_pool pool;
    async_server server_instance(ASYNC_SERVER_TEST_CONFIG);

    boost::thread running_thread(boost::bind(&async_server::run, &server_instance));
    boost::this_thread::sleep(ASYNC_SERVER_SLEEP_TIME);

    boost::thread stopping_thread(boost::bind(&async_server::stop, &server_instance));
    boost::this_thread::sleep(ASYNC_SERVER_SLEEP_TIME);

    stopping_thread.join();
    running_thread.join();
  }

  // run-stop-run-stop from another thread
  {
    NETWORK_MESSAGE("TEST: run-stop-run-stop from another thread");
    util::thread_pool pool;
    async_server server_instance(ASYNC_SERVER_TEST_CONFIG);

    boost::thread running_thread(boost::bind(&async_server::run, &server_instance));
    boost::this_thread::sleep(ASYNC_SERVER_SLEEP_TIME);

    boost::thread stopping_thread(boost::bind(&async_server::stop, &server_instance));
    boost::this_thread::sleep(ASYNC_SERVER_SLEEP_TIME);

    boost::thread second_running_thread(boost::bind(&async_server::run, &server_instance));
    boost::this_thread::sleep(ASYNC_SERVER_SLEEP_TIME);

    boost::thread second_stopping_thread(boost::bind(&async_server::stop, &server_instance));
    boost::this_thread::sleep(ASYNC_SERVER_SLEEP_TIME);

    stopping_thread.join();
    running_thread.join();
    second_stopping_thread.join();
    second_running_thread.join();
  }

  // run-run-stop from another thread
  {
    NETWORK_MESSAGE("TEST: run-run-stop from another thread");
    util::thread_pool pool;
    async_server server_instance(ASYNC_SERVER_TEST_CONFIG);

    boost::thread running_thread(boost::bind(&async_server::run, &server_instance));
    boost::this_thread::sleep(ASYNC_SERVER_SLEEP_TIME);

    boost::thread second_running_thread(boost::bind(&async_server::run, &server_instance));
    boost::this_thread::sleep(ASYNC_SERVER_SLEEP_TIME);

    boost::thread stopping_thread(boost::bind(&async_server::stop, &server_instance));
    boost::this_thread::sleep(ASYNC_SERVER_SLEEP_TIME);

    stopping_thread.join();
    running_thread.join();
    second_running_thread.join();
  }

  // run-stop-stop from another thread
  {
    NETWORK_MESSAGE("TEST: run-stop-stop from another thread");
    util::thread_pool pool;
    async_server server_instance(ASYNC_SERVER_TEST_CONFIG);

    boost::thread running_thread(boost::bind(&async_server::run, &server_instance));
    boost::this_thread::sleep(ASYNC_SERVER_SLEEP_TIME);

    boost::thread stopping_thread(boost::bind(&async_server::stop, &server_instance));
    boost::this_thread::sleep(ASYNC_SERVER_SLEEP_TIME);

    boost::thread second_stopping_thread(boost::bind(&async_server::stop, &server_instance));
    boost::this_thread::sleep(ASYNC_SERVER_SLEEP_TIME);

    stopping_thread.join();
    second_stopping_thread.join();
    running_thread.join();
  }

#undef ASYNC_SERVER_TEST_CONFIG

  return 0;
}
