#ifndef BOOST_NETWORK_PROTOCOL_HTTP_SERVER_PARAMETERS_HPP_20101210
#define BOOST_NETWORK_PROTOCOL_HTTP_SERVER_PARAMETERS_HPP_20101210

// Copyright 2010 Dean Michael Berris.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/protocol/http/parameters.hpp>

namespace boost { namespace network { namespace http {

    BOOST_PARAMETER_NAME(address)
    BOOST_PARAMETER_NAME(port)
    BOOST_PARAMETER_NAME(handler)
    BOOST_PARAMETER_NAME(thread_pool)
    BOOST_PARAMETER_NAME(socket_options)
    
} /* http */
    
} /* network */
    
} /* boost */

#endif /* BOOST_NETWORK_PROTOCOL_HTTP_SERVER_PARAMETERS_HPP_20101210 */
