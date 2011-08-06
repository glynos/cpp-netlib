#            Copyright (c) Dean Michael Berris 2010.
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE_1_0.txt or copy at
#          http://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 2.6)
project(CPP-NETLIB)
find_package( Boost 1.41.0 )
find_package( OpenSSL )
find_package( Threads )
set(CMAKE_VERBOSE_MAKEFILE true)

if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DBOOST_NETWORK_DEBUG)
endif()
if (Boost_FOUND)
    set(Boost_USE_STATIC_LIBS ON)
    set(Boost_USE_MULTI_THREADED ON)
    include_directories(${Boost_INCLUDE_DIRS})
endif(Boost_FOUND)
if (MSVC)
  add_definitions(-D_SCL_SECURE_NO_WARNINGS)
endif(MSVC)
if (WIN32)
  add_definitions(-D_WIN32_WINNT=0x0501)
endif(WIN32)
enable_testing()

if (OPENSSL_FOUND)
    add_definitions(-DBOOST_NETWORK_ENABLE_HTTPS)
endif()

add_subdirectory(libs/network/src)
add_subdirectory(libs/network/test)
if (NOT MSVC)
  add_subdirectory(libs/mime/test)
endif(NOT MSVC)
add_subdirectory(libs/network/example)

