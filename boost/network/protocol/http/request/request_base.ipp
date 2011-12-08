#ifndef BOOST_NETWORK_RPTOCOL_HTTP_REQUEST_BASE_IPP_20111102
#define BOOST_NETWORK_RPTOCOL_HTTP_REQUEST_BASE_IPP_20111102

// Copyright 2011 Dean Michael Berris <dberris@google.com>.
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/protocol/http/request/request_base.hpp>

namespace boost { namespace network { namespace http {

request_base::~request_base() {
  // default implementation, only required for linking.
}

request_storage_base::~request_storage_base() {
  // default implementation, only required for linking.
}
  
} /* http */

} /* network */

} /* boost */

#endif /* BOOST_NETWORK_RPTOCOL_HTTP_REQUEST_BASE_IPP_20111102 */
