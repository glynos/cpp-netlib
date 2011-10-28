#            Copyright (c) Dean Michael Berris 2010.
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE_1_0.txt or copy at
#          http://www.boost.org/LICENSE_1_0.txt)

include_directories(${CPP-NETLIB_SOURCE_DIR})
find_package( Boost 1.41.0 COMPONENTS unit_test_framework system regex date_time thread filesystem )
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)

add_subdirectory(uri)
add_subdirectory(http)

if (Boost_FOUND)
    set(
        TESTS
        message_test
        message_transform_test
        )
    foreach (test ${TESTS})
        set_source_files_properties(${test}.cpp
            PROPERTIES COMPILE_FLAGS "-Wall")
        add_executable(cpp-netlib-${test} ${test}.cpp)
        add_dependencies(cpp-netlib-${test}
            cppnetlib-uri
            cppnetlib-message
            cppnetlib-message-directives
            cppnetlib-message-wrappers)

        target_link_libraries(cpp-netlib-${test}
            ${Boost_LIBRARIES}
            ${CMAKE_THREAD_LIBS_INIT}
            cppnetlib-uri
            cppnetlib-message
            cppnetlib-message-directives
            cppnetlib-message-wrappers)
        if (OPENSSL_FOUND)
            target_link_libraries(cpp-netlib-${test} ${OPENSSL_LIBRARIES})
        endif()
        set_target_properties(cpp-netlib-${test}
            PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CPP-NETLIB_BINARY_DIR}/tests)
        add_test(cpp-netlib-${test} ${CPP-NETLIB_BINARY_DIR}/tests/cpp-netlib-${test})
    endforeach (test)

    set_source_files_properties(utils_thread_pool.cpp
      PROPERTIES COMPILE_FLAGS "-Wall")
    add_executable(cpp-netlib-utils_thread_pool utils_thread_pool.cpp)
    add_dependencies(cpp-netlib-utils_thread_pool
      cppnetlib-utils-thread_pool)
    target_link_libraries(cpp-netlib-utils_thread_pool
      cppnetlib-utils-thread_pool
      ${Boost_LIBRARIES}
      ${CMAKE_THREAD_LIBS_INIT})
    set_target_properties(cpp-netlib-utils_thread_pool
      PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CPP-NETLIB_BINARY_DIR}/tests)
    add_test(cpp-netlib-utils_thread_pool ${CPP-NETLIB_BRINARY_DIR}/tests/cpp-netlib-utils_thread_pool)

endif()

