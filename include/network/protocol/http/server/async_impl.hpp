// Copyright 2012 Dean Michael Berris <dberris@google.com>.
// Copyright 2012 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef NETWORK_PROTOCOL_HTTP_SERVER_ASYNC_IMPL_20120318
#define NETWORK_PROTOCOL_HTTP_SERVER_ASYNC_IMPL_20120318

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/thread/mutex.hpp>
#include <network/protocol/http/server/options.hpp>
#include <network/protocol/http/server/impl/socket_options_setter.hpp>

namespace network { namespace utils {

struct thread_pool;

}  // namespace util

}  // namespace network

namespace network { namespace http {

struct request;

class async_server_connection;

class async_server_impl : protected socket_options_setter {
 public:
  typedef boost::shared_ptr<async_server_connection> connection_ptr;
  async_server_impl(server_options const &options,
                    boost::function<void(request const &, connection_ptr)> handler,
                    utils::thread_pool &thread_pool);
  ~async_server_impl();
  void run();
  void stop();
  void listen();

 private:
  server_options options_;
  std::string address_, port_;
  boost::asio::io_service *service_;
  boost::asio::ip::tcp::acceptor *acceptor_;
  boost::shared_ptr<async_server_connection> new_connection_;
  boost::mutex listening_mutex_, stopping_mutex_;
  boost::function<void(request const &, connection_ptr)> handler_;
  utils::thread_pool &pool_;
  bool listening_, owned_service_, stopping_;

  void handle_stop();
  void start_listening();
  void handle_accept(boost::system::error_code const &ec);
};

}  // namespace http

}  // namespace network

#endif  // NETWORK_PROTOCOL_HTTP_SERVER_ASYNC_IMPL_20120318
