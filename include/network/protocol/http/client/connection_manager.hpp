// Copyright 2011 Dean Michael Berris (dberris@google.com).
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_MANAGER_HPP_20110930
#define NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_MANAGER_HPP_20110930

#include <boost/shared_ptr.hpp>

namespace asio {

class io_service;

}  // namespace asio

namespace network {
namespace http {

struct client_connection;
struct request_base;
class client_options;

struct connection_manager {
  virtual boost::shared_ptr<client_connection> get_connection(
      asio::io_service & service,
      request_base const & request,
      client_options const & options) = 0;
  virtual void clear_resolved_cache() = 0;
  virtual void reset() = 0;
  virtual ~connection_manager() = 0;
};

}  // namespace http
}  // namespace network

#endif /* NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_MANAGER_HPP_20110930 */
