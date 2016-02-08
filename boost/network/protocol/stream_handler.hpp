#ifndef NETLIB_IO_STREAM_HANDLER_HPP
#define NETLIB_IO_STREAM_HANDLER_HPP

// Copyright 2014 Jelle Van den Driessche.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <memory>
#include <asio/async_result.hpp>
#include <asio/basic_socket.hpp>
#include <asio/detail/config.hpp>
#include <asio/detail/handler_type_requirements.hpp>
#include <asio/detail/push_options.hpp>
#include <asio/detail/throw_error.hpp>
#include <asio/error.hpp>
#include <asio/stream_socket_service.hpp>
#include <cstddef>

#ifdef BOOST_NETWORK_ENABLE_HTTPS
#include <asio/ssl.hpp>
#endif

namespace boost {
namespace network {

typedef asio::ip::tcp::socket tcp_socket;

#ifndef BOOST_NETWORK_ENABLE_HTTPS
typedef tcp_socket stream_handler;
typedef void ssl_context;
#else

typedef asio::ssl::stream<asio::ip::tcp::socket> ssl_socket;
typedef asio::ssl::context ssl_context;

struct stream_handler {
 public:
  stream_handler(std::shared_ptr<tcp_socket> socket)
      : tcp_sock_(std::move(socket)), ssl_enabled(false) {}

  ~stream_handler() = default;

  stream_handler(std::shared_ptr<ssl_socket> socket)
      : ssl_sock_(std::move(socket)), ssl_enabled(true) {}

  stream_handler(asio::io_service& io,
                 std::shared_ptr<ssl_context> ctx =
                     std::shared_ptr<ssl_context>()) {
    tcp_sock_ = std::make_shared<tcp_socket>(boost::ref(io));
    ssl_enabled = false;
    if (ctx) {
      /// SSL is enabled
      ssl_sock_ =
          std::make_shared<ssl_socket>(boost::ref(io), boost::ref(*ctx));
      ssl_enabled = true;
    }
  }

  template <typename ConstBufferSequence, typename WriteHandler>
  ASIO_INITFN_RESULT_TYPE(WriteHandler,
                                void(std::error_code, std::size_t))
      async_write_some(const ConstBufferSequence& buffers,
                       ASIO_MOVE_ARG(WriteHandler) handler) {
    try {
      if (ssl_enabled) {
        ssl_sock_->async_write_some(buffers, handler);
      } else {
        tcp_sock_->async_write_some(buffers, handler);
      }
    }
    catch (const std::error_code& e) {
      std::cerr << e.message() << std::endl;
    }
    catch (const std::system_error& e) {
      std::cerr << e.code() << ": " << e.what() << std::endl;
    }
  }

  template <typename MutableBufferSequence, typename ReadHandler>
  ASIO_INITFN_RESULT_TYPE(ReadHandler,
                                void(std::error_code, std::size_t))
      async_read_some(const MutableBufferSequence& buffers,
                      ASIO_MOVE_ARG(ReadHandler) handler) {
    try {
      if (ssl_enabled) {
        ssl_sock_->async_read_some(buffers, handler);
      } else {
        tcp_sock_->async_read_some(buffers, handler);
      }
    }
    catch (const std::error_code& e) {
      std::cerr << e.message() << std::endl;
    }
    catch (const std::system_error& e) {
      std::cerr << e.code() << ": " << e.what() << std::endl;
    }
  }

  void close(std::error_code& e) {
    if (ssl_enabled) {
      ssl_sock_->next_layer().close();
    } else {
      tcp_sock_->close();
    }
  }

  tcp_socket::endpoint_type remote_endpoint() const {
    if (ssl_enabled) {
      return ssl_sock_->next_layer().remote_endpoint();
    } else {
      return tcp_sock_->remote_endpoint();
    }
  }

  void shutdown(asio::socket_base::shutdown_type st,
                std::error_code& e) {
    try {
      if (ssl_enabled) {
        ssl_sock_->shutdown(e);
      } else {
        tcp_sock_->shutdown(asio::ip::tcp::socket::shutdown_send, e);
      }
    }
    catch (const std::error_code& e) {
      std::cerr << e.message() << std::endl;
    }
    catch (const std::system_error& e) {
      std::cerr << e.code() << ": " << e.what() << std::endl;
    }
  }

  ssl_socket::next_layer_type& next_layer() const {
    if (ssl_enabled) {
      return ssl_sock_->next_layer();
    } else {
      return *tcp_sock_;
    }
  }

  ssl_socket::lowest_layer_type& lowest_layer() const {
    if (ssl_enabled) {
      return ssl_sock_->lowest_layer();
    } else {
      return tcp_sock_->lowest_layer();
    }
  }

  template <typename HandshakeHandler>
  ASIO_INITFN_RESULT_TYPE(HandshakeHandler,
                                void(std::error_code))
      async_handshake(ssl_socket::handshake_type type,
                      ASIO_MOVE_ARG(HandshakeHandler) handler) {
    try {
      if (ssl_enabled) {
        return ssl_sock_->async_handshake(type, handler);
      } else {
        // NOOP
      }
    }
    catch (const std::error_code& e) {
      std::cerr << e.message() << std::endl;
    }
    catch (const std::system_error& e) {
      std::cerr << e.code() << ": " << e.what() << std::endl;
    }
  }
  std::shared_ptr<tcp_socket> get_tcp_socket() { return tcp_sock_; }
  std::shared_ptr<ssl_socket> get_ssl_socket() { return ssl_sock_; }

  bool is_ssl_enabled() { return ssl_enabled; }

 private:
  std::shared_ptr<tcp_socket> tcp_sock_;
  std::shared_ptr<ssl_socket> ssl_sock_;
  bool ssl_enabled;
};
#endif
}
}

#endif
