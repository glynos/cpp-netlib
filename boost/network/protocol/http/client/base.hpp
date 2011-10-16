#ifndef BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_BASE_HPP_20111008
#define BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_BASE_HPP_20111008

// Copyright 2011 Dean Michael Berris <dberris@google.com>.
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/function.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/network/protocol/http/client/connection_manager.hpp>
#include <boost/network/protocol/http/response.hpp>
#include <boost/network/protocol/http/request.hpp>

namespace boost { namespace network { namespace http {

struct client_base_pimpl;

struct client_base {
  typedef
    function<void(boost::iterator_range<char const *> const &, system::error_code const &)>
    body_callback_function_type;

  client_base(shared_ptr<connection_manager> connection_manager_);
  client_base(asio::io_service & service,
              shared_ptr<connection_manager> connection_manager_);
  ~client_base();
  response const request_skeleton(request_base const & request_,
                                  std::string const & method,
                                  bool get_body,
                                  body_callback_function_type callback);
  void clear_resolved_cache();
 private:
  client_base_pimpl * pimpl;
};
  
} /* http */
  
} /* network */
  
} /* boost */

#ifdef BOOST_NETWORK_NO_LIB
#include <boost/network/protocol/http/client/base.ipp>
#endif

#endif /* BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_BASE_HPP_20111008 */
