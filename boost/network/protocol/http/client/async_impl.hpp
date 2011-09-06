#ifndef BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_ASYNC_IMPL_HPP_20100623
#define BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_ASYNC_IMPL_HPP_20100623

// Copyright Dean Michael Berris 2010.
// Copyright 2011 Dean Michael Berris (dberris@google.com).
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/asio/io_service.hpp>
#include <boost/asio/strand.hpp>
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>

namespace boost { namespace network { namespace http {

  template <class Tag, unsigned version_major, unsigned version_minor>
  struct basic_client_impl;

  namespace impl {
    template <class Tag, unsigned version_major, unsigned version_minor>
    struct async_client
    {
      typedef
        typename string<Tag>::type
        string_type;

      typedef
        function<void(boost::iterator_range<char const *> const &, system::error_code const &)>
        body_callback_function_type;

      async_client(shared_ptr<connection_manager> connection_manager)
        : service_ptr(new boost::asio::io_service),
        service_(*service_ptr),
        sentinel_(new boost::asio::io_service::work(service_)),
        connection_manager_(connection_manager)
      {
        lifetime_thread_.reset(new boost::thread(
          boost::bind(
            &boost::asio::io_service::run,
            &service_
            )));
      }

      async_client(boost::asio::io_service & service,
                   shared_ptr<connection_manager> connection_manager)
        : service_ptr(0),
        service_(service),
        sentinel_(new boost::asio::io_service::work(service_)),
        connection_manager_(connection_manager)
      {
      }

      ~async_client() throw ()
      {
        sentinel_.reset();
        connection_manager_->reset();
        if (lifetime_thread_.get()) {
          lifetime_thread_->join();
          lifetime_thread_.reset();
        }
        delete service_ptr;
      }

      basic_response<Tag> const request_skeleton(
        basic_request<Tag> const & request_,
        string_type const & method,
        bool get_body,
        body_callback_function_type callback
        )
      {
        shared_ptr<client_connection> connection_;
        connection_ = connection_manager_->get_connection(service_, request_);
        shared_ptr<response_base> response = connection_->send_request(
            method, request_, get_body, callback);
        return *response;
      }

      boost::asio::io_service * service_ptr;
      boost::asio::io_service & service_;
      boost::shared_ptr<boost::asio::io_service::work> sentinel_;
      boost::shared_ptr<boost::thread> lifetime_thread_;
      shared_ptr<connection_manager> connection_manager_;
    };
  } // namespace impl

} // namespace http

} // namespace network

} // namespace boost

#endif // BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_ASYNC_IMPL_HPP_20100623
