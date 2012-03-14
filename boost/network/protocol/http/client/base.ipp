#ifndef BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_ASYNC_IMPL_HPP_20100623
#define BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_ASYNC_IMPL_HPP_20100623

// Copyright 2011 Dean Michael Berris (dberris@google.com).
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/protocol/http/client/base.hpp>
#include <boost/network/protocol/http/client/options.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/strand.hpp>
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include <boost/network/protocol/http/client/connection_manager.hpp>
#include <boost/network/protocol/http/client/simple_connection_manager.hpp>
#include <boost/network/protocol/http/request.hpp>

namespace boost { namespace network { namespace http {

struct client_base_pimpl {
  typedef
    function<void(boost::iterator_range<char const *> const &, system::error_code const &)>
    body_callback_function_type;
  client_base_pimpl(client_options const &options);
  response const request_skeleton(request const & request_,
                                  std::string const & method,
                                  bool get_body,
                                  body_callback_function_type callback,
                                  request_options const &options);
  void clear_resolved_cache();
  ~client_base_pimpl();
 private:
  client_options options_;
  boost::asio::io_service * service_ptr;
  boost::shared_ptr<boost::asio::io_service::work> sentinel_;
  boost::shared_ptr<boost::thread> lifetime_thread_;
  shared_ptr<connection_manager> connection_manager_;
  bool owned_service_;
};

client_base::client_base()
: pimpl(new (std::nothrow) client_base_pimpl(client_options()))
{}

client_base::client_base(client_options const &options)
: pimpl(new (std::nothrow) client_base_pimpl(options))
{}

void client_base::clear_resolved_cache() {
  pimpl->clear_resolved_cache();
}

response const client_base::request_skeleton(request const & request_,
                                             std::string const & method,
                                             bool get_body,
                                             body_callback_function_type callback,
                                             request_options const &options) {
  return pimpl->request_skeleton(request_, method, get_body, callback, options);
}

client_base::~client_base() {
  delete pimpl;
}

client_base_pimpl::client_base_pimpl(client_options const &options)
  : options_(options),
  service_ptr(options.io_service()),
  sentinel_(),
  connection_manager_(options.connection_manager()),
  owned_service_(false) {
  if (service_ptr == 0) {
    service_ptr = new(std::nothrow) asio::io_service;
    owned_service_ = true;
  }
  if (!connection_manager_.get())
    connection_manager_.reset(
        new (std::nothrow) simple_connection_manager(options));
  sentinel_.reset(new (std::nothrow) boost::asio::io_service::work(*service_ptr));
  lifetime_thread_.reset(new (std::nothrow) boost::thread(
    boost::bind(
      &boost::asio::io_service::run,
      service_ptr
      )));
  if (!lifetime_thread_.get())
    BOOST_THROW_EXCEPTION(std::runtime_error("Cannot allocate client lifetime thread; not enough memory."));
}

client_base_pimpl::~client_base_pimpl()
{
  sentinel_.reset();
  connection_manager_->reset();
  if (lifetime_thread_.get()) {
    lifetime_thread_->join();
    lifetime_thread_.reset();
  }
  if (owned_service_) delete service_ptr;
}

response const client_base_pimpl::request_skeleton(
  request const & request_,
  std::string const & method,
  bool get_body,
  body_callback_function_type callback,
  request_options const &options
  )
{
  shared_ptr<client_connection> connection_;
  connection_ = connection_manager_->get_connection(*service_ptr, request_, options_);
  return connection_->send_request(method, request_, get_body, callback, options);
}

void client_base_pimpl::clear_resolved_cache() {
  connection_manager_->clear_resolved_cache();
}

} // namespace http

} // namespace network

} // namespace boost

#endif // BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_ASYNC_IMPL_HPP_20100623
