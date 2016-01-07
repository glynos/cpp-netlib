#ifndef TAG_TYPES_4NNM8B5T
#define TAG_TYPES_4NNM8B5T

// Copyright 2010 Dean Michael Berris.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest-typed-test.h>
#include <boost/network/protocol/http/tags.hpp>

using TagTypes =
    ::testing::Types<boost::network::http::tags::http_default_8bit_tcp_resolve,
                     boost::network::http::tags::http_default_8bit_udp_resolve,
                     boost::network::http::tags::http_async_8bit_tcp_resolve,
                     boost::network::http::tags::http_async_8bit_udp_resolve>;

#endif /* TAG_TYPES_4NNM8B5T */
