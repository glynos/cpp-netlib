#ifndef BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_CONNECTION_FACTORY_HPP_20111112
#define BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_CONNECTION_FACTORY_HPP_20111112

// Copyright 2011 Dean Michael Berris <dberris@google.com>.
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/shared_ptr.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/network/protocol/http/client/client_connection.hpp>

namespace boost { namespace network { namespace http {

struct connection_factory {
  virtual shared_ptr<client_connection> create_connection(asio::io_service & service,
                                                          request_base const & request,
                                                          bool cache_resolved,
                                                          bool follow_redirects,
                                                          optional<std::string> openssl_certificate,
                                                          optional<std::string> openssl_verify_path) = 0;
  virtual ~connection_factory() = 0; // pure virtual, interface only.
};
  
} /* http */
  
} /* network */
  
} /* boost */

#ifdef BOOST_NETWORK_NO_LIB
#include <boost/network/protocol/http/client/connection/connection_factory.ipp>
#endif

#endif /* BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_CONNECTION_FACTORY_HPP_20111112 */
