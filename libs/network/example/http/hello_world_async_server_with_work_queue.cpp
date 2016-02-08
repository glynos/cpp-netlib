/*
 * sample application to show the usage of work queues along with async http
 *server
 *
 * (C) Copyright Dino Korah 2013.
 * Distributed under the Boost Software License, Version 1.0. (See copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include <memory>
#include <mutex>
#include <chrono>
#include <functional>
#include <boost/network/utils/thread_group.hpp>
#include <boost/network/include/http/server.hpp>
#include <boost/network/uri.hpp>
#include <asio.hpp>
#include <iostream>
#include <list>
#include <signal.h>

// This is needed to terminate the worker queue, and must be visible to the signal handler.
bool running = true;

struct handler;
typedef boost::network::http::server<handler> server;

struct server_data {
  boost::network::http::server<handler> server;

  server_data(const server::options &options)
    : server(options) {}

  void run() {
    server.run();
  }

  void stop() {
    running = false;
    server.stop();
  }
};

/**
 * request + connection encapsulation (work item)
 */
struct request_data {
  const server::request req;
  server::connection_ptr conn;

  typedef std::shared_ptr<request_data> pointer;

  request_data(server::request req, server::connection_ptr  conn)
      : req(std::move(req)), conn(std::move(conn)) {}
};

/**
 * A basic work queue
 */
struct work_queue {
  typedef std::list<request_data::pointer> list;

  list requests;
  std::mutex mutex;

  inline void put(const request_data::pointer& request) {
    std::unique_lock<std::mutex> lock(mutex);
    requests.push_back(request);
    (void)lock;
  }

  inline request_data::pointer get() {
    std::unique_lock<std::mutex> lock(mutex);

    request_data::pointer request;
    if (!requests.empty()) {
      request = requests.front();
      requests.pop_front();
    }

    (void)lock;

    return request;
  }
};

struct handler {
  work_queue& queue;

  handler(work_queue& queue) : queue(queue) {}

  /**
   * Feed the work queue
   *
   * @param req
   * @param conn
   */
  void operator()(server::request const& req,
                  const server::connection_ptr& conn) {
    queue.put(std::make_shared<request_data>(req, conn));
  }
};

/**
 * Clean shutdown signal handler
 *
 * @param error
 * @param signal
 * @param server
 */
void shut_me_down(const std::error_code& error, int signal,
                  std::shared_ptr<server_data> server) {
  if (!error) server->stop();
}

/**
 * Process request; worker (thread)
 *
 * @param queue
 */
void process_request(work_queue& queue) {
  while (running) {
    request_data::pointer request(queue.get());
    if (request) {

      // some heavy work!
      std::this_thread::sleep_for(std::chrono::seconds(10));

      request->conn->set_status(server::connection::ok);
      request->conn->write("Hello, world!");
    }

    std::this_thread::sleep_for(std::chrono::microseconds(1000));
  }
}

int main() {
  try {
    // the thread group
    auto threads(std::make_shared<boost::network::utils::thread_group>());

    // setup asio::io_service
    auto io_service(std::make_shared<asio::io_service>());
    auto work(std::make_shared<asio::io_service::work>(std::ref(*io_service)));

    // io_service threads
    {
      int n_threads = 5;
      while (0 < n_threads--) {
        threads->create_thread([=] () { io_service->run(); });
      }
    }

    // the shared work queue
    work_queue queue;

    // worker threads that will process the request; off the queue
    {
      int n_threads = 5;
      while (0 < n_threads--) {
        threads->create_thread([&queue] () { process_request(queue); });
      }
    }

    // setup the async server
    handler request_handler(queue);
    auto server(std::make_shared<server_data>(
        server::options(request_handler)
        .address("0.0.0.0")
        .port("8000")
        .io_service(io_service)
        .reuse_address(true)
        .thread_pool(std::make_shared<boost::network::utils::thread_pool>(
             2, io_service, threads))));

    // setup clean shutdown
    asio::signal_set signals(*io_service, SIGINT, SIGTERM);
    signals.async_wait([=] (std::error_code const &ec, int signal) {
        shut_me_down(ec, signal, server);
      });

    // run the async server
    server->run();

    work.reset();
    io_service->stop();

    threads->join_all();

    std::cout << "Terminated normally" << std::endl;
    exit(EXIT_SUCCESS);
  }
  catch (const std::exception& e) {
    std::cerr << "Abnormal termination - exception:" << e.what() << std::endl;
    exit(EXIT_FAILURE);
  }
}
