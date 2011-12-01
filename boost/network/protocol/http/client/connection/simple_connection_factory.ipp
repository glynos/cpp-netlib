#ifndef BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_SIMPLE_CONNECTION_FACTORY_20111120
#define BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_SIMPLE_CONNECTION_FACTORY_20111120

// Copyright 2011 Dean Michael Berris <dberris@google.com>.
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/protocol/http/client/connection/simple_connection_factory.hpp>
#include <boost/network/protocol/http/client/connection/resolver_delegate_factory.hpp>
#include <boost/network/protocol/http/client/connection/connection_delegate_factory.hpp>
#include <boost/network/protocol/http/client/connection/async_normal.hpp>

namespace boost { namespace network { namespace http {

struct simple_connection_factory_pimpl {
  simple_connection_factory_pimpl(shared_ptr<connection_delegate_factory> conn_delegate_factory,
                                  shared_ptr<resolver_delegate_factory> res_delegate_factory)
  : conn_delegate_factory_(conn_delegate_factory)
  , res_delegate_factory_(res_delegate_factory)
  {}

  shared_ptr<client_connection> create_connection(
      asio::io_service & service,
      request_base const & request,
      bool cache_resolved,
      bool follow_redirects,
      optional<std::string> openssl_certificate,
      optional<std::string> openssl_verify_path) {
    ::boost::network::uri::uri uri_(destination(request));
    bool https = to_lower_copy(scheme(uri_)) == "https";
    shared_ptr<client_connection> conn_;
    conn_.reset(new (std::nothrow) http_async_connection(
      res_delegate_factory_->create_resolver_delegate(service, cache_resolved),
      conn_delegate_factory_->create_connection_delegate(
        service, https, openssl_certificate, openssl_verify_path),
      service,
      follow_redirects));
    return conn_;
  }

 private:
  shared_ptr<connection_delegate_factory> conn_delegate_factory_;
  shared_ptr<resolver_delegate_factory> res_delegate_factory_;
};

simple_connection_factory::simple_connection_factory() {
  shared_ptr<connection_delegate_factory> connection_delegate_factory_;
  connection_delegate_factory_.reset(new (std::nothrow) connection_delegate_factory());
  shared_ptr<resolver_delegate_factory> resolver_delegate_factory_;
  resolver_delegate_factory_.reset(new (std::nothrow) resolver_delegate_factory());
  pimpl.reset(new (std::nothrow) simple_connection_factory_pimpl(
    connection_delegate_factory_, resolver_delegate_factory_));
}

simple_connection_factory::simple_connection_factory(shared_ptr<connection_delegate_factory> conn_delegate_factory,
                                                     shared_ptr<resolver_delegate_factory> res_delegate_factory)
: pimpl(new (std::nothrow) simple_connection_factory_pimpl(conn_delegate_factory, res_delegate_factory))
{}

shared_ptr<client_connection>
simple_connection_factory::create_connection(asio::io_service & service,
                                             request_base const & request,
                                             bool cache_resolved,
                                             bool follow_redirects,
                                             optional<std::string> openssl_certificate,
                                             optional<std::string> openssl_verify_path) {
  return pimpl->create_connection(service, request, cache_resolved, follow_redirects,
                                  openssl_certificate, openssl_verify_path);
}

simple_connection_factory::~simple_connection_factory() {
  // do nothing
}

} /* http */
  
} /* network */
  
} /* boost */

#endif /* BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_SIMPLE_CONNECTION_FACTORY_20111120 */
