#ifndef BOOST_NETWORK_PROTOCOL_HTTP_SERVER_SERVER_IPP_20120318
#define BOOST_NETWORK_PROTOCOL_HTTP_SERVER_SERVER_IPP_20120318

// Copyright 2012 Dean Michael Berris <dberris@google.com>.
// Copyright 2012 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/protocol/http/server/options.hpp>
#include <boost/network/protocol/http/server/sync_impl.hpp>
#include <boost/network/protocol/http/server/async_impl.hpp>

namespace boost { namespace network { namespace http {

template <class SyncHandler>
sync_server<SyncHandler>::sync_server(server_options const &options, SyncHandler &handler)
: pimpl_(new sync_server_impl(options, handler))
{}

template <class SyncHandler>
void sync_server<SyncHandler>::run() {
  pimpl_->run();
}

template <class SyncHandler>
void sync_server<SyncHandler>::stop() {
  pimpl_->stop();
}

template <class SyncHandler>
void sync_server<SyncHandler>::listen() {
  pimpl_->listen();
}

template <class SyncHandler>
sync_server<SyncHandler>::~sync_server() {
  delete pimpl_;
}

template <class AsyncHandler>
async_server<AsyncHandler>::async_server(server_options const &options, AsyncHandler &handler, utils::thread_pool &pool)
: pimpl_(new async_server_impl(options, handler, pool))
{}

template <class AsyncHandler>
void async_server<AsyncHandler>::run() {
  pimpl_->run();
}

template <class AsyncHandler>
void async_server<AsyncHandler>::stop() {
  pimpl_->stop();
}

template <class AsyncHandler>
void async_server<AsyncHandler>::listen() {
  pimpl_->listen();
}

template <class SyncHandler>
async_server<SyncHandler>::~async_server() {
  delete pimpl_;
}

}  // namespace http

}  // namespace network

}  // namespace boost

#endif  // BOOST_NETWORK_PROTOCOL_HTTP_SERVER_SERVER_IPP_20120318
