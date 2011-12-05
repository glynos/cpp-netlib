
# Copyright 2010 Dean Michael Berris.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

include_directories(${CPP-NETLIB_SOURCE_DIR})

if (OPENSSL_FOUND)
    include_directories( ${OPENSSL_INCLUDE_DIR} )
    add_definitions(-DBOOST_NETWORK_ENABLE_HTTPS)
endif()

if (Boost_FOUND)
    set ( BOOST_LIBS
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
        ${Boost_THREAD_LIBRARY}
        ${Boost_SYSTEM_LIBRARY})
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
<<<<<<< HEAD
        add_dependencies(cpp-netlib-http-${test} cppnetlib-uri-parsers)
        target_link_libraries(cpp-netlib-http-${test} ${BOOST_LIBS} ${CMAKE_THREAD_LIBS_INIT} cppnetlib-uri-parsers)
=======
        add_dependencies(cpp-netlib-http-${test} cppnetlib-uri)
        target_link_libraries(cpp-netlib-http-${test} ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} cppnetlib-uri cppnetlib-client-connections)
>>>>>>> cd5b00337224ee166982f4ce954d2d55b906d7da
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
        add_dependencies(cpp-netlib-http-${test} cppnetlib-server-parsers)
        target_link_libraries(cpp-netlib-http-${test} ${BOOST_LIBS} ${CMAKE_THREAD_LIBS_INIT} cppnetlib-server-parsers)
        set_target_properties(cpp-netlib-http-${test}
            PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CPP-NETLIB_BINARY_DIR}/tests)
        add_test(cpp-netlib-http-${test}
            ${CPP-NETLIB_BINARY_DIR}/tests/cpp-netlib-http-${test})
    endforeach (test)

    set ( SERVER_TESTS
        server_hello_world
        server_async
        server_async_less_copy
        )
    set ( PORT 8000 )
    foreach ( test ${SERVER_TESTS} )
        if (${CMAKE_CXX_COMPILER_ID} MATCHES GNU)
            set_source_files_properties(${test}.cpp
                PROPERTIES COMPILE_FLAGS "-Wall")
        endif()
        add_executable(cpp-netlib-http-${test} ${test}.cpp)
        add_dependencies(cpp-netlib-http-${test} cppnetlib-server-parsers)
        target_link_libraries(cpp-netlib-http-${test} ${BOOST_LIBS} ${CMAKE_THREAD_LIBS_INIT} cppnetlib-server-parsers)
        set_target_properties(cpp-netlib-http-${test}
            PROPERTIES RUNTIME_OUTPUT_DIRECTORY
            ${CPP-NETLIB_BINARY_DIR}/tests)
        add_test(cpp-netlib-${test}
            python
            ${CPP-NETLIB_SOURCE_DIR}/libs/network/test/httplib_acceptance.py
            ${CPP-NETLIB_BINARY_DIR}/tests/cpp-netlib-http-${test}
            ${PORT}
            ${CPP-NETLIB_BINARY_DIR}/tests/cpp-netlib-http-${test}.passed)
        math (EXPR PORT "${PORT} + 1")
    endforeach (test)

    set ( INLINED_TESTS
        client_include_inlined
        server_include_inlined
        )
    foreach ( test ${INLINED_TESTS} )
        if (${CMAKE_CXX_COMPILER_ID} MATCHES GNU)
            set_source_files_properties(${test}.cpp
                PROPERTIES COMPILE_FLAGS "-Wall")
        endif()
        add_executable(cpp-netlib-http-inlined-${test} ${test}.cpp)
        target_link_libraries(cpp-netlib-http-inlined-${test} ${BOOST_LIBS} ${CMAKE_THREAD_LIBS_INIT})
        if (OPENSSL_FOUND)
            target_link_libraries(cpp-netlib-http-inlined-${test} ${OPENSSL_LIBRARIES})
        endif()
        set_target_properties(cpp-netlib-http-inlined-${test}
            PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CPP-NETLIB_BINARY_DIR}/tests)
        add_test(cpp-netlib-http-inlined-${test}
            ${CPP-NETLIB_BINARY_DIR}/tests/cpp-netlib-http-inlined-${test})
    endforeach (test)

endif()
