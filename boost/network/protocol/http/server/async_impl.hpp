#ifndef BOOST_NETWORK_PROTOCOL_HTTP_SERVER_ASYNC_IMPL_20120318
#define BOOST_NETWORK_PROTOCOL_HTTP_SERVER_ASYNC_IMPL_20120318

// Copyright 2012 Dean Michael Berris <dberris@google.com>.
// Copyright 2012 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>

namespace boost { namespace network { namespace utils {

struct thread_pool;

}  // namespace util

}  // namespace network

}  // namespace boost

namespace boost { namespace network { namespace http {

class async_server_connection;

class async_server_impl {
 public:
  typedef shared_ptr<async_server_connection> connection_ptr;
  async_server_impl(server_options const &options,
                    function<void(request const &, connection_ptr)> handler,
                    utils::thread_pool &thread_pool);
  void run();
  void stop();
  void listen();

 private:
  server_options options_;
  std::string address_, port_;
  asio::io_service *service_;
  asio::ip::tcp::acceptor *acceptor_;
  shared_ptr<async_server_connection> new_connection_;
  mutex listening_mutex_;
  bool listening_, owned_service_;
  function<void(request const &, connection_ptr)> handler_;
  utils::thread_pool &pool_;
};

}  // namespace http

}  // namespace network

}  // namespace boost

#endif  // BOOST_NETWORK_PROTOCOL_HTTP_SERVER_ASYNC_IMPL_20120318
