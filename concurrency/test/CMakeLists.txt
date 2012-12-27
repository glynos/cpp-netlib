# Copyright (c) Glyn Matthews 2012.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

include_directories(${CPP-NETLIB_SOURCE_DIR}/concurrency/src)

if (${CMAKE_CXX_COMPILER_ID} MATCHES GNU)
  if (HAVE_STD11)
    set(CPP-NETLIB_CXXFLAGS "-Wall -std=c++11")
  elseif (HAVE_STD0X)
    set(CPP-NETLIB_CXXFLAGS "-Wall -std=c++0x")
  endif()
elseif (${CMAKE_CXX_COMPILER_ID} MATCHES Clang)
  CHECK_CXX_COMPILER_FLAG(-std=c++11 HAVE_STD11)
  set(CPP-NETLIB_CXXFLAGS "-Wall -std=c++11 -stdlib=libc++")
  set(CPP-NETLIB_CXXFLAGS "-Wall -std=c++11 -stdlib=libc++")
endif()

set_source_files_properties(thread_pool_test.cpp
  PROPERTIES COMPILE_FLAGS "-Wall")
add_executable(cpp-netlib-thread_pool_test thread_pool_test.cpp)
target_link_libraries(cpp-netlib-thread_pool_test
  cppnetlib-concurrency
  ${Boost_LIBRARIES}
  ${CMAKE_THREAD_LIBS_INIT})
set_target_properties(cpp-netlib-thread_pool_test
  PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CPP-NETLIB_BINARY_DIR}/tests)
add_test(cpp-netlib-thread_pool_test ${CPP-NETLIB_BINARY_DIR}/tests/cpp-netlib-thread_pool_test)
