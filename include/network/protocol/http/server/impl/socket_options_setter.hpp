#ifndef BOOST_NETWORK_PROTOCOL_HTTP_SERVER_IMPL_SOCKET_OPIONS_SETTER_20120319
#define BOOST_NETWORK_PROTOCOL_HTTP_SERVER_IMPL_SOCKET_OPIONS_SETTER_20120319

// Copyright 2012 Dean Michael Berris <dberris@google.com>.
// Copyright 2012 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/asio/ip/tcp.hpp>

namespace boost { namespace network { namespace http {

class server_options;

class socket_options_setter {
 protected:
  void set_socket_options(server_options const &options, asio::ip::tcp::socket &socket);
  void set_acceptor_options(server_options const &options, asio::ip::tcp::acceptor &acceptor);
};

}  // namespace http

}  // namespace network

}  // namespace boost

#endif  // BOOST_NETWORK_PROTOCOL_HTTP_SERVER_IMPL_SOCKET_OPIONS_SETTER_20120319
