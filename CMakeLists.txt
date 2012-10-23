# Copyright (c) Dean Michael Berris 2010.
# Copyright (c) Google, Inc. 2012.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 2.8)
project(CPP-NETLIB)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
find_package( ICU )

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
find_package( Boost 1.45.0 REQUIRED unit_test_framework system regex date_time thread chrono filesystem program_options )
find_package( OpenSSL )
find_package( Threads )
set(CMAKE_VERBOSE_MAKEFILE true)

if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DNETWORK_DEBUG)
endif()

if (OPENSSL_FOUND)
    add_definitions(-DNETWORK_ENABLE_HTTPS)
endif()

if (${CMAKE_CXX_COMPILER_ID} MATCHES GNU)
  INCLUDE(CheckCXXCompilerFlag)
  CHECK_CXX_COMPILER_FLAG(-std=c++0x HAVE_STD0X)
  CHECK_CXX_COMPILER_FLAG(-std=c++11 HAVE_STD11)

  if (HAVE_STD11)
    set(CMAKE_CXX_FLAGS -std=c++11)
  elseif (HAVE_STD0X)
    set(CMAKE_CXX_FLAGS -std=c++0x)
  else()
    message(FATAL_ERROR "No advanced standard C++ support (-std=c++0x and -std=c++11 not defined).")
  endif()
elseif(${CMAKE_CXX_COMPILER_ID} MATCHES Clang)
  set(CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++")
  set(CMAKE_CXX_LINK_FLAGS "-std=c++11 -stdlib=libc++")
  add_definitions(
  -DASIO_HAS_MOVE -DASIO_HAS_VARIADIC_TEMPLATES -DASIO_HAS_STD_SYSTEM_ERROR
  -DASIO_ERROR_CATEGORY_NOEXCEPT=noexcept -DASIO_HAS_STD_ARRAY
  -DASIO_HAS_STD_SHARED_PTR -DASIO_HAS_STD_ATOMIC -DASIO_HAS_STD_CHRONO
  -DASIO_HAS_STD_ADDRESSOFF -DASIO_HAS_STD_FUNCTION -DASIO_HAS_STD_TYPE_TRAITS)
  message("C++ Flags: ${CMAKE_CXX_FLAGS} link flags: ${CMAKE_CXX_LINK_FLAGS}")
endif()

if (Boost_FOUND)
    if (MSVC)
      add_definitions(-D_SCL_SECURE_NO_WARNINGS)
    endif(MSVC)
    if (WIN32)
      add_definitions(-D_WIN32_WINNT=0x0501)
    endif(WIN32)
    include_directories(${Boost_INCLUDE_DIRS})
    enable_testing()
    add_subdirectory(libs/network/src)
    add_subdirectory(libs/network/test)
    if (NOT MSVC)
      add_subdirectory(libs/mime/test)
    endif(NOT MSVC)
    add_subdirectory(libs/network/example)
endif(Boost_FOUND)

enable_testing()
