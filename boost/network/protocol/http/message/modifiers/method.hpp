#ifndef BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_MODIFIERS_METHOD_HPP_20101118
#define BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_MODIFIERS_METHOD_HPP_20101118

// Copyright 2010 Dean Michael Berris. 
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/protocol/http/request_base.hpp>

namespace boost { namespace network { namespace http {
    
inline void method(request_base & request, std::string const & method_) {
  request.set_method(method_);
}

} /* http */
    
} /* network */
    
} /* boost */

#endif /* BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_MODIFIERS_METHOD_HPP_20101118 */
