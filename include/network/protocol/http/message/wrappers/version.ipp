// Copyright 2012 Dean Michael Berris <dberris@google.com>.
// Copyright 2012 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef NETWORK_PROTOCOL_HTTP_MESSAGE_WRAPPERS_VERSION_IPP_20120311
#define NETWORK_PROTOCOL_HTTP_MESSAGE_WRAPPERS_VERSION_IPP_20120311

#include <network/protocol/http/message/wrappers/version.hpp>

namespace network { namespace http {

version_wrapper::version_wrapper(response_base & response)
: response_(response)
{}

version_wrapper::operator std::string() const {
  if (cache_) return *cache_;
  std::string tmp;
  response_.get_version(tmp);
  cache_ = tmp;
  return *cache_;
}

}  // namespace http
}  // namespace network

#endif  // NETWORK_PROTOCOL_HTTP_MESSAGE_WRAPPERS_VERSION_IPP_20120311
