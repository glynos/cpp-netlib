// Copyright 2011 Dean Michael Berris (dberris@google.com).
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_DELEGATE_FACTORY_HPP_20110819
#define NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_DELEGATE_FACTORY_HPP_20110819

#include <boost/throw_exception.hpp>
#include <network/protocol/http/client/connection/connection_delegate.hpp>

namespace network {
namespace http {

class client_options;

struct connection_delegate_factory {
  typedef boost::shared_ptr<connection_delegate> connection_delegate_ptr;

  connection_delegate_factory();

  // This is the factory method that actually returns the delegate instance.
  virtual connection_delegate_ptr create_connection_delegate(
      asio::io_service & service,
      bool https,
      client_options const &options);

  virtual ~connection_delegate_factory();

 private:
  connection_delegate_factory(connection_delegate_factory const &);  // = delete
  connection_delegate_factory& operator=(connection_delegate_factory);  // = delete
};

}  // namespace http
}  // namespace network

#endif /* NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_DELEGATE_FACTORY_HPP_20110819 */
