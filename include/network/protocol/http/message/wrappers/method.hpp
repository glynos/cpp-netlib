#ifndef BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_WRAPPERS_METHOD_HPP_20101118
#define BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_WRAPPERS_METHOD_HPP_20101118

// Copyright 2010 Dean Michael Berris. 
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <network/protocol/http/request/request_base.hpp>

namespace boost { namespace network { namespace http {

struct method_wrapper {
  explicit method_wrapper(request_base & message);
  operator std::string () const;
 private:
  request_base & message_;
};

inline method_wrapper const
method(request_base & message) {
  return method_wrapper(message);
}
    
} /* http */
    
} /* network */
    
} /* boost */

#endif /* BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_WRAPPERS_METHOD_HPP_20101118 */
