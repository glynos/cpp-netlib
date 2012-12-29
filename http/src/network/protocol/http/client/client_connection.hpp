// Copyright 2011 Dean Michael Berris <dberris@google.com>.
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef NETWORK_PROTOCOL_HTTP_CLIENT_CLIENT_CONNECTION_HPP_20111103
#define NETWORK_PROTOCOL_HTTP_CLIENT_CLIENT_CONNECTION_HPP_20111103

#include <functional>
#include <boost/range/iterator_range.hpp>
#include <boost/asio/error.hpp>

namespace network { namespace http {

struct request;
struct response;

class request_options;

struct client_connection {
  typedef std::function<
    void(boost::iterator_range<char const *> const &,
         boost::system::error_code const &)>
    callback_type;
  virtual response send_request(std::string const & method,
                                request const & request,
                                bool get_body,
                                callback_type callback,
                                request_options const &options) = 0;
  virtual client_connection * clone() const = 0;
  virtual void reset() = 0;
  virtual ~client_connection() = 0;
};

} /* http */
  
} /* network */
  
#endif /* NETWORK_PROTOCOL_HTTP_CLIENT_CLIENT_CONNECTION_HPP_20111103 */
