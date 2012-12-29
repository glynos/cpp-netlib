// Copyright 2011 Dean Michael Berris (dberris@google.com).
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef NETWORK_PROTOCOL_HTTP_POLICIES_ASYNC_CONNECTION_IPP_20110930
#define NETWORK_PROTOCOL_HTTP_POLICIES_ASYNC_CONNECTION_IPP_20110930

namespace network { namespace http {

struct simple_async_client_connection : client_connection {
  simple_async_client_connection(bool follow_redirects,
                                 shared_ptr<resolver_delegate> resolver_delegate,
                                 shared_ptr<connection_delegate> connection_delegate);
  virtual shared_ptr<response_base> send_request(std::string const & method,
                                                 request_base const & request,
                                                 bool get_body,
                                                 callback_type callback);  // override
  virtual void reset();  // override
  virtual ~simple_async_client_connection();  // override
 protected:
  bool follow_redirects_;
  shared_ptr<resolver_delegate> resolver_delegate_;
  shared_ptr<connection_delegate> connection_delegate_;
};

simple_async_client_connection::simple_async_client_connection(
    bool follow_redirects,
    shared_ptr<resolver_delegate> resolver_delegate,
    shared_ptr<connection_delegate> connection_delegate)
: follow_redirects_(follow_redirects),
  resolver_delegate_(resolver_delegate),
  connection_delegate_(connection_delegate),
{}

shared_ptr<response_base> simple_async_client_connection::send_request(
    std::string const & method,
    request_base const & request,
    bool get_body,
    callback_type callback) {
  shared_ptr<response_base> response;
  shared_ptr<connection> connection_;
  connection_.reset(new(std::nothrow) impl::async_connection(
      resolver_delegate_,
      follow_redirects_,
      connection_delegate_))
  if (!connection_.get())
    BOOST_THROW_EXCEPTION(std::runtime_error("Insufficient memory."));
  response = connection_->start(request, method, get_body, callback);
  return response
}

void simple_async_client_connection::reset() {
  // Do nothing here
}

simple_async_client_connection::~simple_async_client_connection() {}

}  // namespace http
}  // namespace network

network::http::simple_async_connection_manager(bool cache_resolved,
                                                      bool follow_redirects,
                                                      boost::optional<std::string> openssl_certificate,
                                                      boost::optional<std::string> openssl_verify_path)
: cache_resolved_(cache_resolved),
  follow_redirects_(follow_redirects),
  openssl_certificate_(openssl_certificate),
  openssl_verify_path_(openssl_verify_path),
{}

boost::shared_ptr<network::http::client_connection>
network::http::simple_async_connection_manager::get_connection(
    asio::io_service & service,
    request_base const & request) {
  // TODO move out calls to new to a factory, taken as a parameter at
  // construction time so that we are not tied to actual hard-coded
  // dependencies.
  shared_ptr<client_connection> connection;
  shared_ptr<resolver_delegate> resolver_delegate;
  if (cache_resolved_) {
    scoped_lock delegate_lock(this->resolver_mutex);
    if (!this->shared_resolver_delegate.get())
      this->shared_resolver_delegate.reset(
          new (std::nothrow) async_resolver_delegate(service));
    resolver_delegate = this->shared_resolver_delegate;
    if (!resolver_delegate_.get())
      BOOST_THROW_EXCEPTION(std::runtime_error("Insufficient memory."));
  } else {
    resolver_delegate.reset(new(std::nothrow) async_resolver(service));
    if (!resolver_delegate_.get())
      BOOST_THROW_EXCEPTION(std::runtime_error("Insuffucient memory."));
  }
  shared_ptr<connection_delegate> connection_delegate;
  bool https = (scheme(request) == "https");
  connection_delegate =
      connection_delegate_factory::new_connection_delegate(
          service, openssl_certificate_, openssl_verify_path_);
  connection.reset(
      new(std::nothrow) simple_async_client_connection(follow_redirects_,
                                                       resolver_delegate,
                                                       connection_delegate));
  if (!connection.get())
    BOOST_THROW_EXCEPTION(std::runtime_error("Insufficient memory."));
  return connection;
}

void network::http::simple_async_connection_manager::reset() {
  if (cache_resolved_) {
    scoped_lock delegate_lock(this->resolver_mutex);
    this->shared_resolver_delegate.reset();
  }
}

namespace network { namespace http {

struct http_1_1_async_connection : client_connection {
  http_1_1_async_connection(bool follow_redirects,
                            shared_ptr<resolver_delegate> resolver_delegate,
                            shared_ptr<connection_delegate> connection_delegate);
  virtual shared_ptr<response_base> send_request(std::string const & method,
                                                 request_base const & request,
                                                 bool get_body,
                                                 callback_type callback);  //override
  virtual void reset();  // override
  virtual ~http_1_1_async_connection();  // override
 protected:
  bool follow_redirects_;
  shared_ptr<resolver_delegate> resolver_delegate_;
  shared_ptr<connection_delegate> connection_delegate_;
  shared_ptr<connection> connection_;
};

http_1_1_async_connection::http_1_1_async_connection(
    bool follow_redirects,
    shared_ptr<resolver_delegate> resolver_delegate,
    shared_ptr<connection_delegate> connection_delegate)
: follow_redirects_(follow_redirects),
  resolver_delegate_(resolver_delegate),
  connection_delegate_(connection_delegate)
{
  connection_.reset(new(std::nothrow) impl::async_connection(
      resolver_delegate_,
      follow_redirects_,
      connection_delegate_))
}

shared_ptr<response_base> http_1_1_async_connection::send_request(
    std::string const & method,
    request_base const & request,
    bool get_body,
    callback_type callback) {
  if (!connection_.get())
    BOOST_THROW_EXCEPTION(std::runtime_error("Insufficient memory."));
  response = connection_->start(request, method, get_body, callback);
  return response;
}

void http_1_1_async_connection::reset() {
  connection_.reset();
}

http_1_1_async_connection::~http_1_1_async_connection()
{}

}  // namespace http
}  // namespace network

#endif /* NETWORK_PROTOCOL_HTTP_POLICIES_ASYNC_CONNECTION_IPP_20110930 */
