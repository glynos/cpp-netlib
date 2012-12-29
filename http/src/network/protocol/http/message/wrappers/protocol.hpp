// Copyright 2010 (c) Dean Michael Berris.
// Copyright 2010 (c) Sinefunc, Inc.
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef NETWORK_PROTOCOL_HTTP_MESSAGE_WRAPPERS_PROTOCOL_HPP_20100619
#define NETWORK_PROTOCOL_HTTP_MESSAGE_WRAPPERS_PROTOCOL_HPP_20100619

#include <network/protocol/http/request/request_base.hpp>

namespace network {
namespace http {

struct protocol_wrapper {
  explicit protocol_wrapper(request_base const & request);
  operator std::string () const;
 private:
  request_base const & request_;
};

inline protocol_wrapper const
protocol(request_base const & request) {
  return protocol_wrapper(request);
}

} // namespace http
} // namespace network

#endif // NETWORK_PROTOCOL_HTTP_MESSAGE_WRAPPERS_PROTOCOL_HPP_20100619
