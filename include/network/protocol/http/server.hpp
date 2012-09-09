// Copyright 2009 Tarroo, Inc.
// Copyright 2010 Glyn Matthews
// Copyright 2003-2008 Chris Kholhoff
// Copyright 2009-2012 Dean Michael Berris <dberris@gmail.com>
// Copyright 2012 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_NETWORK_HTTP_SERVER_HPP_
#define BOOST_NETWORK_HTTP_SERVER_HPP_

#include <boost/shared_ptr.hpp>

namespace boost { namespace network { namespace utils {

struct thread_pool;

}  // namespace utils

}  // namespace network

}  // namespace boost

namespace boost { namespace network { namespace http {

class server_options;
class sync_server_impl;
class async_server_impl;
class async_server_connection;
struct request;
struct response;


template <class SyncHandler>
class sync_server {
 public:
  sync_server(server_options const &options, SyncHandler &handler);
  void run();
  void stop();
  void listen();
  ~sync_server();

  typedef http::request request;
  typedef http::response response;
 private:
  sync_server_impl *pimpl_;
  sync_server(sync_server const &other);  // = delete
  sync_server& operator=(sync_server other);  // = delete
};

template <class AsyncHandler>
class async_server {
 public:
  explicit async_server(server_options const &options, AsyncHandler &handler, utils::thread_pool &pool);
  void run();
  void stop();
  void listen();
  ~async_server();

  typedef http::request request;
  typedef shared_ptr<async_server_connection> connection_ptr;
 private:
  async_server_impl *pimpl_;
  async_server(async_server const &other);  // = delete
  async_server& operator=(async_server other);  // = delete
};

} // namespace http

} // namespace network

} // namespace boost

// We're hiding the implementation from here, but still explicitly including
// it here. This is mostly a style point, to keep this header clean.
#include <network/protocol/http/server/server.ipp>

#endif // BOOST_NETWORK_HTTP_SERVER_HPP_

