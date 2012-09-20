#ifndef NETWORK_DEBUG_HPP_20110410
#define NETWORK_DEBUG_HPP_20110410

// (c) Copyright 2011 Dean Michael Berris.
// Copyright 2012 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/** NETWORK_MESSAGE is a debugging macro used by cpp-netlib to
    print out network-related errors through standard error. This is only
    useful when NETWORK_DEBUG is turned on. Otherwise the macro amounts to a
    no-op.
*/
#ifdef NETWORK_DEBUG
#  include <iostream>
#  ifndef NETWORK_MESSAGE
#    define NETWORK_MESSAGE(msg) std::cerr << "[DEBUG " << __FILE__ << ':' << __LINE__ << "]: " << msg << std::endl;
#  endif
#else
#  ifndef NETWORK_MESSAGE
#    define NETWORK_MESSAGE(msg)
#  endif
#endif

#endif /* end of include guard: NETWORK_DEBUG_HPP_20110410 */
