#ifndef BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_MANAGER_HPP_20110930
#define BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_MANAGER_HPP_20110930

// Copyright 2011 Dean Michael Berris (dberris@google.com).
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/protocol/http/client/client_connection.hpp>
#include <boost/network/protocol/http/request/request_base.hpp>
#include <boost/network/protocol/http/response/response_base.hpp>
#include <boost/asio/io_service.hpp>

namespace boost { namespace network { namespace http {

struct connection_manager {
  virtual shared_ptr<client_connection> get_connection(
      asio::io_service & service,
      request_base const & request) = 0;
  virtual void reset() = 0;
  virtual ~connection_manager() = 0;
};

} /* http */

} /* network */

} /* boost */

#ifdef BOOST_NETWORK_NO_LIB
#include <boost/network/protocol/http/client/connection_manager.ipp>
#endif

#endif /* BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_MANAGER_HPP_20110930 */
