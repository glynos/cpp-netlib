#ifndef BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_PARAMETERS_HPP_2010127
#define BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_PARAMETERS_HPP_2010127

// Copyright 2010 Dean Michael Berris. 
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/parameter.hpp>

namespace boost { namespace network { namespace http {

    BOOST_PARAMETER_NAME(follow_redirects)
    BOOST_PARAMETER_NAME(io_service)
    BOOST_PARAMETER_NAME(cache_resolved)
    
} /* http */
    
} /* network */
    
} /* boost */

#endif /* BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_PARAMETERS_HPP_2010127 */
