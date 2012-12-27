# Copyright (c) Dean Michael Berris 2010.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

include_directories(${CPP-NETLIB_SOURCE_DIR}/include)

if(CPP-NETLIB_BUILD_SHARED_LIBS)
    add_definitions(-DCPP-NETLIB_BUILD_SHARED_LIBS)
endif()

add_subdirectory(http)
