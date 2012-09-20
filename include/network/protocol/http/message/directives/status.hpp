// Copyright 2010 (c) Sinefunc, Inc.
// Copyright 2011 Dean Michael Berris <dberris@google.com>.
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef NETWORK_PROTOCOL_HTTP_MESSAGE_DIRECTIVES_STATUS_HPP_20100603
#define NETWORK_PROTOCOL_HTTP_MESSAGE_DIRECTIVES_STATUS_HPP_20100603

#include <boost/thread/future.hpp>
#include <boost/mpl/if.hpp>
#include <boost/cstdint.hpp>
#include <network/protocol/http/response/response_base.hpp>

namespace network {
namespace http {

struct status_directive {
  explicit status_directive(std::string const & s);
  void operator()(response_base & response) const;

 protected:
  std::string status_;
};

inline status_directive status(std::string const & response) {
  return status_directive(response);
}

}  // namespace http
}  // namespace network

#endif // NETWORK_PROTOCOL_HTTP_MESSAGE_DIRECTIVES_STATUS_HPP_20100603
