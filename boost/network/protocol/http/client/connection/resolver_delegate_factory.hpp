#ifndef BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_RESOLVER_DELEGATE_FACTORY_HPP_20110930
#define BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_RESOLVER_DELEGATE_FACTORY_HPP_20110930

// Copyright 2011 Dean Michael Berris (dberris@google.com).
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/shared_ptr.hpp>
#include <boost/network/protocol/http/request/request_base.hpp>
#include <boost/network/protocol/http/client/connection/resolver_delegate.hpp>

namespace boost { namespace network { namespace http {

struct resolver_delegate_factory {
  resolver_delegate_factory();
  virtual shared_ptr<resolver_delegate> create_resolver_delegate(
      asio::io_service & service,
      request_base const & request);
  virtual ~resolver_delegate_factory();
 private:
  resolver_delegate_factory(resolver_delegate_factory const &);  // = delete
  resolver_delegate_factory& operator=(resolver_delegate_factory);  // = delete
};

} /* http */
} /* network */
} /* boost */

#ifdef BOOST_NETWORK_NO_LIB
#include <boost/network/protocol/http/client/connection/resolver_deleagte_factory.ipp>
#endif

#endif /* BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_RESOLVER_DELEGATE_FACTORY_HPP_20110930 */
