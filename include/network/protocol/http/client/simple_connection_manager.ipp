#ifndef BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_SIMPLE_CONNECTION_MANAGER_IPP_20111112
#define BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_SIMPLE_CONNECTION_MANAGER_IPP_20111112

// Copyright 2011 Dean Michael Berris <dberris@google.com>.
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <network/protocol/http/client/simple_connection_manager.hpp>
#include <network/protocol/http/client/connection/simple_connection_factory.hpp>
#include <network/protocol/http/client/options.hpp>
#include <network/detail/debug.hpp>

namespace boost { namespace network { namespace http {

struct simple_connection_manager_pimpl {
  simple_connection_manager_pimpl(client_options const &options)
  : options_(options)
  , connection_factory_(options.connection_factory())
  {
    BOOST_NETWORK_MESSAGE(
        "simple_connection_manager_pimpl::simple_connection_manager_pimpl("
        "client_options const &)");
    if (!connection_factory_.get()) {
      BOOST_NETWORK_MESSAGE("creating simple connection factory");
      connection_factory_.reset(
          new (std::nothrow) simple_connection_factory());
    }
  }

  shared_ptr<client_connection> get_connection(asio::io_service & service,
                                               request_base const & request,
                                               client_options const &options) {
    BOOST_NETWORK_MESSAGE("simple_connection_manager_pimpl::get_connection(...)");
    return connection_factory_->create_connection(service, request, options_);
  }

  void reset() {
    // do nothing here.
  }

  void clear_resolved_cache() {
    // TODO(deanberris): Make this happen!
  }

  ~simple_connection_manager_pimpl() {
    BOOST_NETWORK_MESSAGE("simple_connection_manager_pimpl::~simple_connection_manager_pimpl()");
    // do nothing here.
  }

private:
  client_options options_;
  shared_ptr<connection_factory> connection_factory_;
};

simple_connection_manager::simple_connection_manager(client_options const &options)
: pimpl(new (std::nothrow) simple_connection_manager_pimpl(options))
{
  BOOST_NETWORK_MESSAGE("simple_connection_manager::simple_connection_manager("
                        "client_options const &)");
}

shared_ptr<client_connection> simple_connection_manager::get_connection(
    asio::io_service & service,
    request_base const & request,
    client_options const &options) {
  BOOST_NETWORK_MESSAGE("simple_connection_manager::get_connection(...)");
  return pimpl->get_connection(service, request, options);
}

void simple_connection_manager::reset() {
  BOOST_NETWORK_MESSAGE("simple_connection_manager::reset()");
  pimpl->reset();
}

void simple_connection_manager::clear_resolved_cache() {
  BOOST_NETWORK_MESSAGE("simple_connection_manager::clear_resolved_cache()");
  pimpl->clear_resolved_cache();
}

simple_connection_manager::~simple_connection_manager() {
  BOOST_NETWORK_MESSAGE("simple_connection_manager::~simple_connection_manager()");
  // do nothing here.
}

} /* http */

} /* network */

} /* boost */

#endif /* BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_SIMPLE_CONNECTION_MANAGER_IPP_20111112 */
