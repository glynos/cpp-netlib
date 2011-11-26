#ifndef BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_ASYNC_RESOLVER_IPP_20110911
#define BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_ASYNC_RESOLVER_IPP_20111126

// Copyright 2011 Dean Michael Berris <dberris@google.com>.
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <string>
#include <utility>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/udp.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/asio/strand.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/unordered_map.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/fusion/tuple.hpp>
#include <boost/fusion/adapted/std_pair.hpp>
#include <boost/network/protocol/http/client/connection/async_resolver.hpp>

namespace boost { namespace network { namespace http {
struct async_resolver_pimpl : enable_shared_from_this<async_resolver_pimpl> {
  typedef resolver_delegate::resolve_completion_function resolve_completion_function;
  async_resolver_pimpl(asio::io_service & service, bool cache_resolved);
  void resolve(std::string const & host,
               uint16_t port,
               resolve_completion_function once_resolved);
  void clear_resolved_cache();
 private:
  asio::ip::udp::resolver resolver_;
  bool cache_resolved_;
  typedef asio::ip::udp::resolver::iterator
          resolver_iterator;
  typedef unordered_map<std::string, std::pair<resolver_iterator,resolver_iterator> >
          endpoint_cache;
  endpoint_cache endpoint_cache_;
  scoped_ptr<asio::io_service::strand> resolver_strand_;

  void handle_resolve(std::string const & host,
                      resolve_completion_function once_resolved,
                      system::error_code const & ec,
                      resolver_iterator endpoint_iterator);
};
  
async_resolver_pimpl::async_resolver_pimpl(asio::io_service & service, bool cache_resolved)
  : resolver_(service),
    cache_resolved_(cache_resolved),
    endpoint_cache_(),
    resolver_strand_(new(std::nothrow) asio::io_service::strand(service))
{
  // Do nothing
}

void async_resolver_pimpl::clear_resolved_cache() {
  if (cache_resolved_)
    endpoint_cache().swap(endpoint_cache_);
}

void async_resolver_pimpl::resolve(std::string const & host,
                                   boost::uint16_t port,
                                   resolve_completion_function once_resolved) {
  if (!resolver_strand_.get())
    BOOST_THROW_EXCEPTION(std::runtime_error(
        "Uninitialized resolver strand, ran out of memory."));

  if (cache_resolved_) {
    endpoint_cache::iterator iter =
        endpoint_cache_.find(boost::to_lower_copy(host));
    if (iter != endpoint_cache_.end()) {
      boost::system::error_code ignored;
      once_resolved(ignored, iter->second);
      return;
    }
  }

  std::string port_str = lexical_cast<std::string>(port);
  asio::ip::udp::resolver::query query(host, port_str);
  resolver_.async_resolve(
      query,
      resolver_strand_->wrap(
          boost::bind(
              &async_resolver_pimpl::handle_resolve,
              async_resolver_pimpl::shared_from_this(),
              boost::to_lower_copy(host),
              once_resolved,
              boost::asio::placeholders::error,
              boost::asio::placeholders::iterator)));
}

void async_resolver_pimpl::handle_resolve(std::string const & host,
                                          resolve_completion_function once_resolved,
                                          boost::system::error_code const & ec,
                                          resolver_iterator endpoint_iterator) {
  endpoint_cache::iterator iter;
  bool inserted = false;
  if (!ec && cache_resolved_) {
    boost::fusion::tie(iter, inserted) =
      endpoint_cache_.insert(
        std::make_pair(host,
                       std::make_pair(endpoint_iterator,
                                      resolver_iterator())));
    once_resolved(ec, iter->second);
  } else {
    once_resolved(ec, std::make_pair(endpoint_iterator,resolver_iterator()));
  }
}

async_resolver::async_resolver(asio::io_service & service, bool cache_resolved)
: pimpl(new (std::nothrow) async_resolver_pimpl(service, cache_resolved))
{}

void async_resolver::resolve(std::string const & host,
                             uint16_t port,
                             resolve_completion_function once_resolved) {
  BOOST_ASSERT(pimpl.get() && "Uninitialized pimpl, probably ran out of memory.");
  pimpl->resolve(host, port, once_resolved);
}

void async_resolver::clear_resolved_cache() {
  BOOST_ASSERT(pimpl.get() && "Uninitialized pimpl, probably ran out of memory.");
  pimpl->clear_resolved_cache();
}

async_resolver::~async_resolver() {
  // Do nothing
}

} /* http */
  
} /* network */
  
} /* boost */


#endif /* BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_ASYNC_RESOLVER_IPP_20111126 */
