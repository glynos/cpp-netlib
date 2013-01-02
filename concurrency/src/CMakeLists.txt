# Copyright (c) Glyn Matthews 2012.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

include_directories(${CPP-NETLIB_SOURCE_DIR}/concurrency/src)

if (${CMAKE_CXX_COMPILER_ID} MATCHES GNU)
  set(CPP-NETLIB_CXXFLAGS "-Wall -std=c++11")
elseif (${CMAKE_CXX_COMPILER_ID} MATCHES Clang)
  CHECK_CXX_COMPILER_FLAG(-std=c++11 HAVE_STD11)
  set(CPP-NETLIB_CXXFLAGS "-Wall -std=c++11 -stdlib=libc++")
  set(CPP-NETLIB_CXXFLAGS "-Wall -std=c++11 -stdlib=libc++")
endif()

set(CPP-NETLIB_CONCURRENCY_SRCS thread_pool.cpp)
add_library(cppnetlib-concurrency ${CPP-NETLIB_CONCURRENCY_SRCS})
foreach (src_file ${CPP-NETLIB_CONCURRENCY_SRCS})
if (${CMAKE_CXX_COMPILER_ID} MATCHES GNU)
    set_source_files_properties(${src_file}
        PROPERTIES COMPILE_FLAGS ${CPP-NETLIB_CXXFLAGS})
elseif (${CMAKE_CXX_COMPILER_ID} MATCHES Clang)
    set_source_files_properties(${src_file}
        PROPERTIES COMPILE_FLAGS ${CPP-NETLIB_CXXFLAGS})
endif()
endforeach(src_file)
