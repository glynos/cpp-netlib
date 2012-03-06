#ifndef BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_FACADE_HPP_20100623
#define BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_FACADE_HPP_20100623

// Copyright Dean Michael Berris 2010.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/protocol/http/request.hpp>
#include <boost/network/protocol/http/response.hpp>
#include <boost/network/protocol/http/client/base.hpp>
#include <boost/network/protocol/http/client/options.hpp>

namespace boost { namespace network { namespace http {

struct basic_client_facade {
  typedef client_base::body_callback_function_type body_callback_function_type;

  basic_client_facade();
  explicit basic_client_facade(client_options const &options);

  response const head(request const &request, request_options const&options=request_options());
  response const get(request const &request,
                     body_callback_function_type body_handler = body_callback_function_type(),
                     request_options const &options=request_options());
  response const post(request request,
                      optional<std::string> body = optional<std::string>(),
                      optional<std::string> content_type = optional<std::string>(),
                      body_callback_function_type body_handler = body_callback_function_type(),
                      request_options const&options = request_options());
  response const put(request request,
                     optional<std::string> body = optional<std::string>(),
                     optional<std::string> content_type = optional<std::string>(),
                     body_callback_function_type body_handler = body_callback_function_type(),
                     request_options const & options = request_options());
  response const delete_(request const & request,
                         body_callback_function_type body_handler = body_callback_function_type(),
                         request_options const & options = request_options());
  void clear_resolved_cache();


 protected:
  boost::scoped_ptr<client_base> base;
};

} // namespace http

} // namespace network

} // namespace boost

#endif // BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_FACADE_HPP_20100623
