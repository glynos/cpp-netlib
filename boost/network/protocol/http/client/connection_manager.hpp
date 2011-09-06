#ifndef BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_MANAGER_HPP_20110930
#define BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_MANAGER_HPP_20110930

// Copyright 2011 Dean Michael Berris (dberris@google.com).
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

namespace boost { namespace network { namespace http {

struct request_base;

struct response_base;

struct client_connection {
  typedef function<void(iterator_range<char const *> const &,
                        system::error_code const &)>
      callback_type;
  virtual shared_ptr<response_base> send_request(std::string const & method,
                                                 request_base const & request,
                                                 bool get_body,
                                                 callback_type callback) = 0;
  virtual void reset() = 0;
  virtual ~client_connection() = 0;
};

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

#endif /* BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_MANAGER_HPP_20110930 */
