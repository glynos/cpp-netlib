/*
 * Sample application based loosely on existing async server sample to
 *demonstrate ssl
 *
 * Requires openssl lib to run (https://www.openssl.org/)
 *
 * (C) Copyright Jelle Van den Driessche 2014.
 *
 * Distributed under the Boost Software License, Version 1.0. (See copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include <memory>
#include <boost/network/include/http/server.hpp>

#include <asio.hpp>
#include <asio/ssl.hpp>
#include <iostream>
#include <signal.h>

struct handler;
typedef boost::network::http::server<handler> server;

std::string password_callback(
    std::size_t max_length,
    asio::ssl::context_base::password_purpose purpose) {
  return std::string("test");
}

/**
 * request + connection encapsulation (work item)
 */
struct request_data {
  const server::request req;
  server::connection_ptr conn;

  typedef std::shared_ptr<request_data> pointer;

  request_data(server::request const& req, const server::connection_ptr& conn)
      : req(req), conn(conn) {}
};

struct handler {
  void operator()(server::request const& req,
                  const server::connection_ptr& conn) {
    conn->set_status(server::connection::ok);
    conn->write(std::string("Hello World!!"));
  }
};

/**
 * Clean shutdown signal handler
 *
 * @param error
 * @param signal
 * @param p_server_instance
 */
void shut_me_down(const std::error_code& error, int signal,
                  std::shared_ptr<server> p_server_instance) {
  if (!error) p_server_instance->stop();
}

int main(void) try {

  // setup asio::io_service
  std::shared_ptr<asio::io_service> p_io_service(
      std::make_shared<asio::io_service>());

  // Initialize SSL context
  std::shared_ptr<asio::ssl::context> ctx =
      std::make_shared<asio::ssl::context>(
          asio::ssl::context::sslv23);
  ctx->set_options(asio::ssl::context::default_workarounds |
                   asio::ssl::context::no_sslv2 |
                   asio::ssl::context::single_dh_use);

  // Set keys
  ctx->set_password_callback(password_callback);
  ctx->use_certificate_chain_file("server.pem");
  ctx->use_private_key_file("server.pem", asio::ssl::context::pem);
  ctx->use_tmp_dh_file("dh2048.pem");

  // setup the async server
  handler request_handler;
  std::shared_ptr<server> p_server_instance(std::make_shared<server>(
      server::options(request_handler)
          .address("0.0.0.0")
          .port("8442")
          .io_service(p_io_service)
          .reuse_address(true)
          .thread_pool(
               std::make_shared<boost::network::utils::thread_pool>(2))
          .context(ctx)));

  // setup clean shutdown
  asio::signal_set signals(*p_io_service, SIGINT, SIGTERM);
  signals.async_wait([=] (std::error_code const &ec, int signal) {
      shut_me_down(ec, signal, p_server_instance);
    });

  // run the async server
  p_server_instance->run();

  // we are stopped - shutting down

  p_io_service->stop();

  std::cout << "Terminated normally" << std::endl;
  exit(EXIT_SUCCESS);
}
catch (const std::exception& e) {
  std::cout << "Abnormal termination - exception:" << e.what() << std::endl;
  exit(EXIT_FAILURE);
}
