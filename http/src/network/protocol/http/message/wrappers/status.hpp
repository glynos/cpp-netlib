// Copyright 2010 (c) Dean Michael Berris
// Copyright 2010 (c) Sinefunc, Inc.
// Copyright 2012 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef NETWORK_PROTOCOL_HTTP_MESSAGE_WRAPPERS_STATUS_HPP_20100603
#define NETWORK_PROTOCOL_HTTP_MESSAGE_WRAPPERS_STATUS_HPP_20100603

#include <boost/cstdint.hpp>

namespace network { namespace http {

struct status_wrapper {
  explicit status_wrapper(response_base & response);
  operator uint16_t () const;
 private:
  response_base & response_;
};

inline
status_wrapper const
status(response_base & response) {
    return status_wrapper(response);
}

} // namespace http
} // namespace network

#endif // NETWORK_PROTOCOL_HTTP_MESSAGE_WRAPPERS_STATUS_HPP_20100603
