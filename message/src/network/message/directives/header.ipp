// Copyright 2011 Dean Michael Berris (dberris@google.com).
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef NETWORK_MESSAGE_DIRECTIVES_IPP_20111021
#define NETWORK_MESSAGE_DIRECTIVES_IPP_20111021

#include <string>
#include <network/message/directives/header.hpp>

namespace network { namespace impl {

header_directive::header_directive(std::string const & name,
                                   std::string const & value):
  name_(name),
  value_(value) {}

void header_directive::operator() (message_base & msg) const {
  msg.append_header(name_, value_);
}

}  // namespace impl
}  // namespace network

#endif /* NETWORK_MESSAGE_DIRECTIVES_IPP_20111021 */
