# Copyright (c) Glyn Matthews 2012.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

if (${CMAKE_CXX_COMPILER_ID} MATCHES GNU)
  set(CPP-NETLIB_CXXFLAGS "-Wall -std=c++11")
elseif (${CMAKE_CXX_COMPILER_ID} MATCHES Clang)
  CHECK_CXX_COMPILER_FLAG(-std=c++11 HAVE_STD11)
  set(CPP-NETLIB_CXXFLAGS "-Wall -std=c++11 -stdlib=libc++")
  set(CPP-NETLIB_CXXFLAGS "-Wall -std=c++11 -stdlib=libc++")
endif()

include_directories(${CPP-NETLIB_SOURCE_DIR}/mime/src)

if ( Boost_FOUND )
    add_executable ( mime-roundtrip mime-roundtrip.cpp )
    target_link_libraries ( mime-roundtrip )
    add_test ( mime-roundtrip mime-roundtrip )
endif ()

