#ifndef BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_CLIENT_CONNECTION_HPP_20111103
#define BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_CLIENT_CONNECTION_HPP_20111103

// Copyright 2011 Dean Michael Berris <dberris@google.com>.
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/function.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/system/error_code.hpp>

namespace boost { namespace network { namespace http {

struct request;
struct response;

class request_options;

struct client_connection {
  typedef function<void(iterator_range<char const *> const &,
                        system::error_code const &)>
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
  
} /* boost */

#ifdef BOOST_NETWORK_NO_LIB
#include <boost/network/protocol/http/client/client_connection.ipp>
#endif

#endif /* BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_CLIENT_CONNECTION_HPP_20111103 */
