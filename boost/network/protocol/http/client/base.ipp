#ifndef BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_ASYNC_IMPL_HPP_20100623
#define BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_ASYNC_IMPL_HPP_20100623

// Copyright 2011 Dean Michael Berris (dberris@google.com).
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/asio/io_service.hpp>
#include <boost/asio/strand.hpp>
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include <boost/network/protocol/http/client/connection_manager.hpp>

namespace boost { namespace network { namespace http {

struct client_base_pimpl {
  typedef
    function<void(boost::iterator_range<char const *> const &, system::error_code const &)>
    body_callback_function_type;
  client_base_pimpl(shared_ptr<connection_manager> connection_manager_);
  client_base_pimpl(io_service & service, shared_ptr<connection_manager> connection_manager_);
  response const request_skeleton(request const & request_,
                                  std::string const & method,
                                  bool get_body,
                                  body_callback_function_type callback);
  ~client_base_pimpl();
 private:
  boost::asio::io_service * service_ptr;
  boost::asio::io_service & service_;
  boost::shared_ptr<boost::asio::io_service::work> sentinel_;
  boost::shared_ptr<boost::thread> lifetime_thread_;
  shared_ptr<connection_manager> connection_manager_;
};

client_base::client_base(shared_ptr<connection_manager> connection_manager_)
: pimpl(new (std::nothrow) client_base_pimpl(connection_manager_))
{}

client_base::client_base(asio::io_service & service,
                         shared_ptr<connection_manager> connection_manager_)
: pimpl(new (std::nothrow) client_base_pimpl(service, connection_manager_))
{}

response const client_base::request_skeleton(request_base const & request_,
                                             std::string const & method,
                                             bool get_body,
                                             body_callback_function_type callback) {
  return pimpl->request_skeleton(request_, method, get_body, callback);
}

client_base::~client_base() {
  delete pimpl;
}

client_base_pimpl::client_base_pimpl(shared_ptr<connection_manager> connection_manager)
  : service_ptr(new (std::nothrow) boost::asio::io_service),
  service_(*service_ptr),
  sentinel_(new (std::nothrow) boost::asio::io_service::work(service_)),
  connection_manager_(connection_manager)
{
  lifetime_thread_.reset(new (std::nothrow) boost::thread(
    boost::bind(
      &boost::asio::io_service::run,
      &service_
      )));
  if (!lifetime_thread_.get())
    BOOST_THROW_EXCEPTION(std::runtime_error("Cannot allocate client lifetime thread; not enough memory."));
}

client_base_pimpl::client_base_pimpl(boost::asio::io_service & service,
             shared_ptr<connection_manager> connection_manager)
  : service_ptr(0),
  service_(service),
  sentinel_(new (std::nothrow) boost::asio::io_service::work(service_)),
  connection_manager_(connection_manager)
{
  if (!sentinel_.get())
    BOOST_THROW_EXCEPTION(std::runtime_error("Cannot allocate sentinel; not enough memory."));
}

~client_base_pimpl::client_base_pimpl()
{
  sentinel_.reset();
  connection_manager_->reset();
  if (lifetime_thread_.get()) {
    lifetime_thread_->join();
    lifetime_thread_.reset();
  }
  delete service_ptr;
}

response const client_base_pimpl::request_skeleton(
  request const & request_,
  std::string const & method,
  bool get_body,
  body_callback_function_type callback
  )
{
  shared_ptr<client_connection> connection_;
  connection_ = connection_manager_->get_connection(service_, request_);
  return connection_->send_request(method, request_, get_body, callback);
}

} // namespace http

} // namespace network

} // namespace boost

#endif // BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_ASYNC_IMPL_HPP_20100623
