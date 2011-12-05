#            Copyright (c) Dean Michael Berris 2010.
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE_1_0.txt or copy at
#          http://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 2.6)
project(CPP-NETLIB)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTI_THREADED ON)
find_package( Boost 1.43.0 REQUIRED unit_test_framework system regex date_time thread filesystem )
find_package( OpenSSL )
find_package( Threads )
set(CMAKE_VERBOSE_MAKEFILE true)

if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DBOOST_NETWORK_DEBUG)
endif()

if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()
enable_testing()
add_subdirectory(libs/network/src)
add_subdirectory(libs/network/test)
add_subdirectory(libs/mime/test)
add_subdirectory(libs/network/example)
