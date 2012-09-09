#ifndef BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_MODIFIERS_STATUS_MESSAGE_HPP_20100608
#define BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_MODIFIERS_STATUS_MESSAGE_HPP_20100608

// Copyright 2010 (c) Dean Michael Berris
// Copyright 2010 (c) Sinefunc, Inc.
// Copyright 2012 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <network/protocol/http/response/response_base.hpp>

namespace boost { namespace network { namespace http {

inline void status_message(response_base & response, std::string const & value) {
  response.set_status_message(value);
}

} // namespace http

} // namespace network

} // namespace boost

#endif // BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_MODIFIERS_STATUS_MESSAGE_HPP_20100608
