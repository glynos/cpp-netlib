#ifndef BOOST_NETWORK_MESSAGE_WRAPPERS_DESTINATION_IPP_20111021
#define BOOST_NETWORK_MESSAGE_WRAPPERS_DESTINATION_IPP_20111021

// Copyright 2011 Dean Michael Berris (dberris@google.com).
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/message/wrappers/destination.hpp>

namespace boost { namespace network { namespace impl {

destination_wrapper::destination_wrapper(message_base const & message):
  message_(message) {}

destination_wrapper::operator std::string () const {
  if (cache_) {
    return *cache_;
  }
  std::string tmp;
  message_.get_destination(tmp);
  cache_ = tmp;
  return *cache_;
}

} /* impl */

} /* network */

} /* boost */

#endif /* BOOST_NETWORK_MESSAGE_WRAPPERS_DESTINATION_IPP_20111021 */
