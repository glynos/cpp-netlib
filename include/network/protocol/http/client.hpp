// Copyright 2012 Dean Michael Berris <dberris@google.com>.
// Copyright 2012 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef NETWORK_PROTOCOL_HTTP_CLIENT_20091215
#define NETWORK_PROTOCOL_HTTP_CLIENT_20091215

#include <network/version.hpp>
#include <network/protocol/http/client/options.hpp>

#include <boost/asio/io_service.hpp>
#include <network/protocol/http/client/facade.hpp>
#include <network/protocol/http/client/macros.hpp>
#include <network/protocol/http/request.hpp>
#include <network/protocol/http/response.hpp>

namespace network {
namespace http {

struct client : basic_client_facade {
 private:
  typedef basic_client_facade
          base_facade_type;
 public:
  typedef ::boost::network::http::request request;
  typedef ::boost::network::http::response response;

  // Constructor
  // =================================================================
  // A client can be default constructed.
  client();
  // This is a simplified constructor that takes a reference to a const
  // client_options instance. To find out what the supported options are
  // see the boost/network/protocol/http/client/options.hpp file.
  //
  explicit client(client_options const &options);
  //
  // =================================================================
};

} // namespace http
} // namespace network

#endif // NETWORK_PROTOCOL_HTTP_CLIENT_20091215
