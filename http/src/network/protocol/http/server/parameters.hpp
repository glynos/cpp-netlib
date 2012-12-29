// Copyright 2010 Dean Michael Berris.
// Copyright 2012 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef NETWORK_PROTOCOL_HTTP_SERVER_PARAMETERS_HPP_20101210
#define NETWORK_PROTOCOL_HTTP_SERVER_PARAMETERS_HPP_20101210

#include <network/protocol/http/parameters.hpp>

namespace network { namespace http {

    BOOST_PARAMETER_NAME(address)
    BOOST_PARAMETER_NAME(port)
    BOOST_PARAMETER_NAME(handler)
    BOOST_PARAMETER_NAME(thread_pool)

    BOOST_PARAMETER_NAME(reuse_address)
    BOOST_PARAMETER_NAME(report_aborted)
    BOOST_PARAMETER_NAME(receive_buffer_size)
    BOOST_PARAMETER_NAME(send_buffer_size)
    BOOST_PARAMETER_NAME(receive_low_watermark)
    BOOST_PARAMETER_NAME(send_low_watermark)
    BOOST_PARAMETER_NAME(non_blocking_io)
    BOOST_PARAMETER_NAME(linger)
    BOOST_PARAMETER_NAME(linger_timeout)
    
}  // namespace http
}  // namespace network
    
#endif /* NETWORK_PROTOCOL_HTTP_SERVER_PARAMETERS_HPP_20101210 */
