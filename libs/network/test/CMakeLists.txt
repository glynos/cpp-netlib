# Copyright (c) Dean Michael Berris 2010.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

include_directories(${CPP-NETLIB_SOURCE_DIR}/include)

if(CPP-NETLIB_BUILD_SHARED_LIBS)
    add_definitions(-DCPP-NETLIB_BUILD_SHARED_LIBS)
endif()

add_subdirectory(http)

if (Boost_FOUND)

    set_source_files_properties(utils_thread_pool.cpp
      PROPERTIES COMPILE_FLAGS "-Wall")
    add_executable(cpp-netlib-utils_thread_pool utils_thread_pool.cpp)
    target_link_libraries(cpp-netlib-utils_thread_pool
      cppnetlib-utils-thread_pool
      ${Boost_LIBRARIES}
      ${CMAKE_THREAD_LIBS_INIT})
    set_target_properties(cpp-netlib-utils_thread_pool
      PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CPP-NETLIB_BINARY_DIR}/tests)
    add_test(cpp-netlib-utils_thread_pool ${CPP-NETLIB_BINARY_DIR}/tests/cpp-netlib-utils_thread_pool)

endif()

