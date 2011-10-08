#ifndef BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_20091215
#define BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_20091215

//          Copyright Dean Michael Berris 2007-2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/version.hpp>

#include <boost/asio/io_service.hpp>
#include <boost/network/protocol/http/client/facade.hpp>
#include <boost/network/protocol/http/client/parameters.hpp>
#include <boost/network/protocol/http/client/macros.hpp>
#include <boost/network/protocol/http/request.hpp>
#include <boost/network/protocol/http/response.hpp>

namespace boost { namespace network { namespace http {

struct client : basic_client_facade {
 private:
  typedef basic_client_facade
          base_facade_type;
 public:
  typedef ::boost::network::http::request request;
  typedef ::boost::network::http::response response;

  // Constructor
  // =================================================================
  // This is a Boost.Parameter-based constructor forwarder, whose
  // implementation is actually forwarded to the base type.
  //
  // The supported parameters are:
  //      _follow_redirects : bool -- whether to follow HTTP redirect
  //                                  responses (default: false)
  //      _cache_resolved   : bool -- whether to cache the resolved
  //                                  endpoints (default: false)
  //      _io_service       : boost::asio::io_service &
  //                               -- supply an io_service to the
  //                                  client
  //      _openssl_certificate : string
  //                               -- the name of the certificate file
  //                                  to use
  //      _openssl_verify_path : string
  //                               -- the name of the directory from
  //                                  which the certificate authority
  //                                  files can be found
  //      _connection_manager : shared_ptr<connection_manager>
  //                              -- The connection manager to use for
  //                                 this client.

  BOOST_PARAMETER_CONSTRUCTOR(
      basic_client, (base_facade_type), tag,
      (optional
          (in_out(io_service), (boost::asio::io_service&))
          (follow_redirects, (bool))
          (cache_resolved, (bool))
          (openssl_certificate, (string_type))
          (openssl_verify_path, (string_type))
          (connection_manager, (shared_ptr<connection_manager>))
          ))

  //
  // =================================================================

};

} // namespace http

} // namespace network

} // namespace boost

#endif // BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_20091215
