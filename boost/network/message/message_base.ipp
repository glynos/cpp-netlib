#ifndef BOOST_NETWORK_MESSAGE_BASE_IPP_20111020
#define BOOST_NETWORK_MESSAGE_BASE_IPP_20111020

// Copyright 2011 Dean Michael Berris (dberris@google.com).
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/message_base.hpp>

namespace boost { namespace network {

message_base::~message_base() {
  // This is never used, but is required even though message_base's destructor
  // is a pure virtual one.
}

} /* network */

} /* boost */

#endif /* BOOST_NETWORK_MESSAGE_BASE_IPP_20111020 */
