// Copyright 2011 Dean Michael Berris (dberris@google.com).
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// This is the directives file where all standard directives on messages are
// pulled in and compiled into a library.

#ifdef BOOST_NETWORK_NO_LIB
#undef BOOST_NETWORK_NO_LIB
#endif

#include <network/message/directives/header.ipp>
#include <network/message/directives/remove_header.ipp>
