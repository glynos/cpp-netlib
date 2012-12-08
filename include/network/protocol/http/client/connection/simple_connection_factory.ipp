// Copyright 2011 Dean Michael Berris <dberris@google.com>.
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_SIMPLE_CONNECTION_FACTORY_20111120
#define NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_SIMPLE_CONNECTION_FACTORY_20111120

#include <memory>
#include <network/protocol/http/client/connection/simple_connection_factory.hpp>
#include <network/protocol/http/client/connection/resolver_delegate_factory.hpp>
#include <network/protocol/http/client/connection/connection_delegate_factory.hpp>
#include <network/protocol/http/client/connection/async_normal.hpp>
#include <network/protocol/http/client/options.hpp>
#include <network/detail/debug.hpp>
#ifdef NETWORK_DEBUG
#include <network/uri/uri_io.hpp>
#endif
#include <boost/algorithm/string/case_conv.hpp>

#include <network/protocol/http/message/wrappers/uri.hpp>

namespace network {
namespace http {

struct simple_connection_factory_pimpl {
  simple_connection_factory_pimpl(std::shared_ptr<connection_delegate_factory> conn_delegate_factory,
                                  std::shared_ptr<resolver_delegate_factory> res_delegate_factory)
  : conn_delegate_factory_(conn_delegate_factory)
  , res_delegate_factory_(res_delegate_factory) {
    NETWORK_MESSAGE("simple_connection_factory_pimpl::simple_connection_factory_pimpl(...)");
  }

  std::shared_ptr<client_connection> create_connection(
      boost::asio::io_service & service,
      request_base const & request,
      client_options const & options) {
    NETWORK_MESSAGE("simple_connection_factory_pimpl::create_connection(...)");
    ::network::uri uri_ = http::uri(request);
    NETWORK_MESSAGE("destination: " << uri_);
    bool https = boost::algorithm::to_lower_copy(::network::scheme(uri_)) == "https";
    return std::make_shared<http_async_connection>(
      res_delegate_factory_->create_resolver_delegate(service, options.cache_resolved()),
      conn_delegate_factory_->create_connection_delegate(service, https, options),
      service,
      options.follow_redirects());
  }

 private:
  std::shared_ptr<connection_delegate_factory> conn_delegate_factory_;
  std::shared_ptr<resolver_delegate_factory> res_delegate_factory_;
};

simple_connection_factory::simple_connection_factory() {
  NETWORK_MESSAGE("simple_connection_factory::simple_connection_factory()");
  std::shared_ptr<connection_delegate_factory> connection_delegate_factory_;
  connection_delegate_factory_.reset(new (std::nothrow) connection_delegate_factory());
  std::shared_ptr<resolver_delegate_factory> resolver_delegate_factory_;
  resolver_delegate_factory_.reset(new (std::nothrow) resolver_delegate_factory());
  pimpl.reset(new (std::nothrow) simple_connection_factory_pimpl(
    connection_delegate_factory_, resolver_delegate_factory_));
}

simple_connection_factory::simple_connection_factory(std::shared_ptr<connection_delegate_factory> conn_delegate_factory,
                                                     std::shared_ptr<resolver_delegate_factory> res_delegate_factory)
: pimpl(new (std::nothrow) simple_connection_factory_pimpl(conn_delegate_factory, res_delegate_factory))
{
  NETWORK_MESSAGE("simple_connection_factory::simple_connection_factory(...)");
}

std::shared_ptr<client_connection>
simple_connection_factory::create_connection(boost::asio::io_service & service,
                                             request_base const & request,
                                             client_options const &options) {
  NETWORK_MESSAGE("simple_connection_factory::create_connection(...)");
  return pimpl->create_connection(service, request, options);
}

simple_connection_factory::~simple_connection_factory() {
  NETWORK_MESSAGE("simple_connection_factory::~simple_connection_factory()");
  // do nothing
}

}  // namespace http
}  // namespace network

#endif /* NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_SIMPLE_CONNECTION_FACTORY_20111120 */
