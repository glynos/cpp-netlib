// Copyright 2010 Dean Michael Berris.
// Copyright 2012 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef NETWORK_PROTOCOL_HTTP_MESSAGE_HEADER_VALUE_HPP_20101028
#define NETWORK_PROTOCOL_HTTP_MESSAGE_HEADER_VALUE_HPP_20101028

#include <utility>
#include <string>

namespace network { namespace http {

struct request_header;
struct response_header;

template <class T1, class T2>
inline T1 const & value(std::pair<T1,T2> const & p) {
  return p.second;
}

inline std::string const &
value(request_header const & h) {
  return h.value;
}

inline std::string const &
value(response_header const & h) {
  return h.value;
}

}  // namespace http
}  // namespace network
  
#endif /* NETWORK_PROTOCOL_HTTP_MESSAGE_HEADER_VALUE_HPP_20101028 */

