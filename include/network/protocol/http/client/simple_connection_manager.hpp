#ifndef BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_SIMPLE_CONNECTION_MANAGER_HPP_20111105
#define BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_SIMPLE_CONNECTION_MANAGER_HPP_20111105

// Copyright 2011 Dean Michael Berris <dberris@google.com>.
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/scoped_ptr.hpp>
#include <network/protocol/http/client/connection_manager.hpp>
#include <network/protocol/http/client/connection/connection_factory.hpp>

namespace boost { namespace network { namespace http {

/// Forward declaration of simple_connection_manager_pimpl.
struct simple_connection_manager_pimpl;

/// Forward declaration of the client_options class.
class client_options;

/** simple_connection_manager
 *  
 *  This connection manager implementation doesn't do any connection tracking
 *  and mostly delegates the connection creation from a connection factory.
 */
struct simple_connection_manager : connection_manager {
  /** Constructor
   *
   *  Args:
   *    options: A properly construction client_options instance.
   */
  explicit simple_connection_manager(client_options const &options);

  /** get_connection
   *
   * Args:
   *   asio::io_service & service: The io_service instance to which the
   *                               connection should be bound to (for newly
   *                               created objects, ignored for already
   *                               created objects).
   *   request_base const & request: The request object that includes the
   *                                 information required by the connection.
   *   client_options const & options: The options relating to the client
   *                                   options.
   *
   * Returns:
   *   shared_ptr<client_connection> -- either an already-generated object
   *   or a newly constructed connection configured to perform the request.
   */
  virtual shared_ptr<client_connection> get_connection(
      asio::io_service & service,
      request_base const & request,
      client_options const & options);  // override

  /** reset
   *
   * This function resets the internal caches and intermediary data structures
   * used by the connection manager to perform its functions. In the case of
   * this simple_connection_manager, the call to reset will not do anything.
   */
  virtual void reset();  // override

  /** clear_resolved_cache
   *
   * This function resets all the resolved endpoints that have been cached.
   */
  virtual void clear_resolved_cache();

  /** Destructor.
   */
  virtual ~simple_connection_manager();  // override

 protected:
  scoped_ptr<simple_connection_manager_pimpl> pimpl;

 private:
  /// Disabled copy constructor.
  simple_connection_manager(simple_connection_manager const &); // = delete
  /// Disabled assignment operator.
  simple_connection_manager & operator=(simple_connection_manager); // = delete
};

} /* http */

} /* network */

} /* boost */

#endif /* BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_SIMPLE_CONNECTION_MANAGER_HPP_20111105 */
