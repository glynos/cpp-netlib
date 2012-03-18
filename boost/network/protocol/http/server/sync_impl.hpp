#ifndef BOOST_NETWORK_PROTOCOL_HTTP_SERVER_IMPL_HPP_20120318
#define BOOST_NETWORK_PROTOCOL_HTTP_SERVER_IMPL_HPP_20120318

// Copyright 2012 Dean Michael Berris <dberris@google.com>.
// Copyright 2012 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/asio/ip/tcp.hpp>

namespace boost { namespace network { namespace http {

class sync_server_connection;
struct request;
struct response;

class sync_server_impl {
 public:
  sync_server_impl(server_options const &options,
                   function<void(request const &, response &)> handler);
  void run();
  void stop();
  void listen();

 private:
  server_options options_;
  std::string address_, port_;
  asio::io_service *service_;
  asio::ip::tcp::acceptor *acceptor_;
  shared_ptr<sync_server_connection> new_connection_;
  mutex listening_mutex_;
  bool listening_, owned_service_;
  function<void(request const &, response &)> handler_;
};

}  // namespace http

}  // namespace network

}  // namespace boost

#endif  // BOOST_NETWORK_PROTOCOL_HTTP_SERVER_IMPL_HPP_20120318
