// Copyright Dean Michael Berris 2009.
// Copyright Glyn Matthews 2010.
// Copyright 2012 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef NETWORK_VERSION_HPP_20091214
#define NETWORK_VERSION_HPP_20091214

#include <boost/config.hpp>

#define NETLIB_VERSION_MAJOR 1
#define NETLIB_VERSION_MINOR 0
#define NETLIB_VERSION_INCREMENT 0a

#ifndef NETLIB_VERSION
#define NETLIB_VERSION \
    BOOST_STRINGIZE(NETLIB_VERSION_MAJOR) "." \
    BOOST_STRINGIZE(NETLIB_VERSION_MINOR) "." \
    BOOST_STRINGIZE(NETLIB_VERSION_INCREMENT)
#endif // NETLIB_VERSION
#endif // NETWORK_VERSION_HPP_20091214
