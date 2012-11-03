// Copyright Dean Michael Berris 2012.
// Copyright Google, Inc. 2012.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef NETWORK_PROTOCOL_HTTP_CLIENT_OPTIONS_HPP
#define NETWORK_PROTOCOL_HTTP_CLIENT_OPTIONS_HPP

#include <list>
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/asio/io_service.hpp>
#include <network/protocol/http/client/connection/connection_factory.hpp>
#include <network/protocol/http/client/connection_manager.hpp>
#include <network/protocol/http/client/client_connection.hpp>

namespace network { namespace http {

  // Forward-declare the pimpl.
  class client_options_pimpl;

  // This file defines all the options supported by the HTTP client
  // implementation.
  class client_options {
  public:
    client_options();
    client_options(client_options const &other);  // Copy constructible.
    client_options(client_options &&other);  // Move constructible.
    client_options& operator=(client_options const &rhs);  // Copy assignable.
    client_options& operator=(client_options &&rhs);  // Move assignable.
    void swap(client_options &other);  // Swappable.
    ~client_options();  // Non-virtual destructor by design.

    // When adding more supported options, follow the pattern:
    //
    //   client_options& name_of_option(type variable);
    //   type name_of_option() const;
    //
    // These names have to be self-documenting but should still be documented
    // to signify their intent and reason for being an option. The reason for
    // returning a client_options& (usually *this) is so that it the operations
    // can be chained properly. An example of usage would be:
    //
    //   client_options options;
    //   options.io_service(&my_io_service)
    //          .follow_redirects()
    //          .cache_resolved();
    //   client client_(options);

    // These are the setter and getter for the Boost.Asio io_service to use.
    // The default setting when un-set is nullptr, meaning it signals the client
    // implementation that the user doesn't want to use his own io_service
    // instance.
    client_options& io_service(boost::asio::io_service *io_service);
    boost::asio::io_service* io_service() const;

    // The following option determines whether the client should follow
    // HTTP redirects when the implementation encounters them. The default
    // behavior is to return false.
    client_options& follow_redirects(bool setting=false);
    bool follow_redirects() const;

    // The following options determines whether the client should cache
    // resolved endpoints. The default behavior is to not cache resolved
    // endpoints.
    client_options& cache_resolved(bool setting=true);
    bool cache_resolved() const;

    // The following options provide the OpenSSL certificate paths to use.
    // Setting these options without OpenSSL support is valid, but the client
    // may throw an exception when attempting to make SSL connections. The
    // default implementation doesn't define certificate paths.
    client_options& add_openssl_certificate_path(std::string const &path);
    std::list<std::string> const & openssl_certificate_paths() const;

    // The following options provide the OpenSSL certificate authority paths
    // where the certificates can be found. Setting these options without OpenSSL
    // support is valid, but the client may throw an exception when attempting
    // to make SSL connections. The default implementation doesn't define
    // certificate authority paths.
    client_options& add_openssl_verify_path(std::string const &path);
    std::list<std::string> const & openssl_verify_paths() const;

    // The following options provide the connection manager shared pointer that
    // is what the client will use to manage connections.
    client_options& connection_manager(boost::shared_ptr<http::connection_manager> manager);
    boost::shared_ptr<http::connection_manager> connection_manager() const;

    // The following supports providing the connection factory instance responsible
    // for creating the correct instances of the appropriate connection.
    client_options& connection_factory(boost::shared_ptr<http::connection_factory> factory);
    boost::shared_ptr<http::connection_factory> connection_factory() const;

    // More options go here...

  private:
    // We hide the options in a pimpl, so that when new options get added
    // we can keep backward binary compatibility and re-link to the new
    // supported options without having to break those
    client_options_pimpl *pimpl;
  };

  // Forward declare the request_options pimpl.
  class request_options_pimpl;

  // This is the per-request options we allow users to provide. These allow
  // for defining operational options that control a request's flow.
  class request_options {
  public:
    request_options();  // Default constructor.
    request_options(request_options const &other);  // Copy constructible.
    request_options& operator=(request_options rhs);  // Assignable.
    void swap(request_options &other);  // Swappable.
    ~request_options();  // Non-virtual destructor by design.

    // We follow the same pattern as the client_options class and use the
    // chaining call pattern to set the options. When adding new options
    // to support, they should look roughly like so:
    //
    //   request_options& name_of_option(type_of_option variable=default_value);
    //   type_of_option name_of_option() const;
    //
    // See client_options above for a usage example in the same vein.

    // These determine the timeout when performing requests. The default timeout
    // is 30,000 milliseconds (30 seconds).
    request_options& timeout(uint64_t milliseconds = 30 * 1000);
    uint64_t timeout() const;

    // These determine the maximum number of redirects to follow. The default
    // implementation uses 10 as the maximum. A negative value means to keep
    // following redirects until they no longer redirect.
    request_options& max_redirects(int redirects=10);
    int max_redirects() const;

    // More options go here...

  private:
    // For the same reason as the client_options being a pimpl, we do this for
    // minimizing the need to break ABI compatibility when adding new supported
    // options.
    request_options_pimpl *pimpl;
  };

} // namespace http
} // namespace network

#endif  // NETWORK_PROTOCOL_HTTP_CLIENT_OPTIONS_HPP
