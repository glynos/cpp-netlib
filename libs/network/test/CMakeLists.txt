#            Copyright (c) Dean Michael Berris 2010.
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE_1_0.txt or copy at
#          http://www.boost.org/LICENSE_1_0.txt)

include_directories(${CPP-NETLIB_SOURCE_DIR})
find_package( Boost 1.41.0 COMPONENTS unit_test_framework system regex date_time thread filesystem )
find_package( OpenSSL )
if (OPENSSL_FOUND)
    include_directories(${OPENSSL_INCLUDE_DIR}})
    add_definitions(-DBOOST_NETWORK_ENABLE_HTTPS)
endif()
find_package( Threads )
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)

if (Boost_FOUND)
    set(
        TESTS 
        http_incremental_parser 
        http_1_0_test
        http_1_1_test
        http_localhost_tests
        https_localhost_tests
        message_test
        http_message_test
        message_transform_test
        url_test
        utils_thread_pool
        )
    foreach (test ${TESTS})
        set_source_files_properties(${test}.cpp
            PROPERTIES COMPILE_FLAGS "-Wall")
        add_executable(cpp-netlib-${test} ${test}.cpp)
        target_link_libraries(cpp-netlib-${test} ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
        if (OPENSSL_FOUND)
            target_link_libraries(cpp-netlib-${test} ${OPENSSL_LIBRARIES})
        endif()
        set_target_properties(cpp-netlib-${test}
            PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../../../build/tests)
        add_test(cpp-netlib-${test} ../../../build/tests/cpp-netlib-${test})
    endforeach (test)

    set(SERVER_TESTS
        hello_world
        http_async_server
        )
    foreach (test ${SERVER_TESTS})
        set_source_files_properties(${test}.cpp
            PROPERTIES COMPILE_FLAGS "-Wall")
        add_executable(cpp-netlib-${test} ${test}.cpp)
        target_link_libraries(cpp-netlib-${test} ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
        set_target_properties(cpp-netlib-${test}
            PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../../../build/tests)
        add_test(cpp-netlib-${test} python httplib_acceptance.py ../../../build/tests/cpp-netlib-${test} ../../../build/tests/cpp-netlib-${test}.passed)
    endforeach (test)

endif()

