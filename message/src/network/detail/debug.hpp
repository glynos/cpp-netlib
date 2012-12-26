#ifndef NETWORK_DEBUG_HPP_20110410
#define NETWORK_DEBUG_HPP_20110410

// (c) Copyright 2011 Dean Michael Berris.
// Copyright 2012 Google, Inc.
// Copyright 2012 A. Joel Lamotte
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/** NETWORK_MESSAGE is a debugging macro used by cpp-netlib to
    print out network-related errors through standard error. This is only
    useful when NETWORK_DEBUG is turned on. Otherwise the macro amounts to a
    no-op.

    The user can force the logging to be enabled by defining NETWORK_ENABLE_LOGGING.
*/
#if defined(NETWORK_DEBUG) && !defined(NETWORK_ENABLE_LOGGING)
#  define NETWORK_ENABLE_LOGGING
#endif

#ifdef NETWORK_ENABLE_LOGGING

#  include <network/logging/logging.hpp>
#  ifndef NETWORK_MESSAGE
#  define NETWORK_MESSAGE(msg) { network::logging::log( network::logging::log_record( __FILE__, __LINE__ ) << msg ); }
#  endif

#else

#  ifndef NETWORK_MESSAGE
#    define NETWORK_MESSAGE(msg)
#  endif

#endif


#endif /* end of include guard: NETWORK_DEBUG_HPP_20110410 */
