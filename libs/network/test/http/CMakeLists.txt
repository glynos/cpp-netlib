
# Copyright 2010 Dean Michael Berris.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

include_directories(${CPP-NETLIB_SOURCE_DIR}/include)
include_directories(${CPP-NETLIB_SOURCE_DIR})

if (OPENSSL_FOUND)
    include_directories( ${OPENSSL_INCLUDE_DIR} )
    add_definitions(-DBOOST_NETWORK_ENABLE_HTTPS)
endif()

if (Boost_FOUND)
    # These are the internal (simple) tests.
    set ( MESSAGE_TESTS
        request_base_test
        request_test
        request_linearize_test
        response_test
        )
    foreach ( test ${MESSAGE_TESTS} )
        if (${CMAKE_CXX_COMPILER_ID} MATCHES GNU)
            set_source_files_properties(${test}.cpp
                PROPERTIES COMPILE_FLAGS "-Wall")
        endif()
        add_executable(cpp-netlib-http-${test} ${test}.cpp)
        add_dependencies(cpp-netlib-http-${test}
            cppnetlib-message
            cppnetlib-message-wrappers
            cppnetlib-http-message
            cppnetlib-http-message-wrappers
            cppnetlib-uri
            cppnetlib-constants
	  )
        target_link_libraries(cpp-netlib-http-${test}
            ${Boost_LIBRARIES}
	    ${CMAKE_THREAD_LIBS_INIT}
            cppnetlib-message
            cppnetlib-message-wrappers
            cppnetlib-http-message
            cppnetlib-http-message-wrappers
            cppnetlib-uri
            cppnetlib-constants
            )
        set_target_properties(cpp-netlib-http-${test}
            PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CPP-NETLIB_BINARY_DIR}/tests)
        add_test(cpp-netlib-http-${test}
            ${CPP-NETLIB_BINARY_DIR}/tests/cpp-netlib-http-${test})
    endforeach(test)

    set ( TESTS
        client_constructor_test
        client_get_test
        client_get_different_port_test
        client_get_timeout_test
        client_get_streaming_test
        )
    foreach ( test ${TESTS} )
        if (${CMAKE_CXX_COMPILER_ID} MATCHES GNU)
            set_source_files_properties(${test}.cpp
                PROPERTIES COMPILE_FLAGS "-Wall")
        endif()
        add_executable(cpp-netlib-http-${test} ${test}.cpp)
        target_link_libraries(cpp-netlib-http-${test}
            ${Boost_LIBRARIES}
            ${CMAKE_THREAD_LIBS_INIT}
            cppnetlib-constants
            cppnetlib-uri
            cppnetlib-message
            cppnetlib-message-wrappers
            cppnetlib-message-directives
            cppnetlib-http-message
            cppnetlib-http-message-wrappers
            cppnetlib-http-client
            cppnetlib-http-client-connections)
        if (OPENSSL_FOUND)
            target_link_libraries(cpp-netlib-http-${test} ${OPENSSL_LIBRARIES})
        endif()
        set_target_properties(cpp-netlib-http-${test}
            PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CPP-NETLIB_BINARY_DIR}/tests)
        add_test(cpp-netlib-http-${test}
            ${CPP-NETLIB_BINARY_DIR}/tests/cpp-netlib-http-${test})
    endforeach (test)

    set ( SERVER_API_TESTS
        server_constructor_test
        server_async_run_stop_concurrency
        )
    foreach ( test ${SERVER_API_TESTS} )
        if (${CMAKE_CXX_COMPILER_ID} MATCHES GNU)
            set_source_files_properties(${test}.cpp
                PROPERTIES COMPILE_FLAGS "-Wall")
        endif()
        add_executable(cpp-netlib-http-${test} ${test}.cpp)
        target_link_libraries(cpp-netlib-http-${test}
            ${Boost_LIBRARIES}
            ${CMAKE_THREAD_LIBS_INIT}
            cppnetlib-constants
            cppnetlib-uri
            cppnetlib-message
            cppnetlib-message-wrappers
            cppnetlib-http-message
            cppnetlib-http-server
            cppnetlib-http-server-parsers
            cppnetlib-utils-thread_pool
            )
        set_target_properties(cpp-netlib-http-${test}
            PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CPP-NETLIB_BINARY_DIR}/tests)
        add_test(cpp-netlib-http-${test}
            ${CPP-NETLIB_BINARY_DIR}/tests/cpp-netlib-http-${test})
    endforeach (test)

endif()
