
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
    set(TESTS response_incremental_parser_test request_incremental_parser_test
      request_linearize_test)
    foreach ( test ${TESTS} )
        add_executable(cpp-netlib-http-${test} ${test}.cpp)
        add_dependencies(cpp-netlib-http-${test} gtest_main
            cppnetlib-uri)
        target_link_libraries(cpp-netlib-http-${test}
            ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} gtest_main
            ${CMAKE_THREAD_LIBS_INIT} gtest_main
            cppnetlib-uri)
	if (${CMAKE_CXX_COMPILER_ID} MATCHES GNU AND ${CMAKE_SYSTEM_NAME} MATCHES "Windows")
          target_link_libraries(cpp-netlib-http-${test} ws2_32)
        endif()
	if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	  target_link_libraries(cpp-netlib-http-${test} rt)
	endif()
        set_target_properties(cpp-netlib-http-${test}
            PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CPP-NETLIB_BINARY_DIR}/tests)
        add_test(cpp-netlib-http-${test}
            ${CPP-NETLIB_BINARY_DIR}/tests/cpp-netlib-http-${test})
    endforeach (test)
    set(TESTS
      client_constructor_test
      client_get_test
      client_get_different_port_test
      # client_get_timeout_test
      client_get_ready_test
      client_get_streaming_test)
    foreach ( test ${TESTS} )
        add_executable(cpp-netlib-http-${test} ${test}.cpp)
        add_dependencies(cpp-netlib-http-${test} cppnetlib-uri
	  cppnetlib-client-connections gtest_main)
        target_link_libraries(cpp-netlib-http-${test}
	  ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} cppnetlib-uri gtest_main cppnetlib-client-connections)
        if (OPENSSL_FOUND)
            target_link_libraries(cpp-netlib-http-${test} ${OPENSSL_LIBRARIES})
        endif()
	if (${CMAKE_CXX_COMPILER_ID} MATCHES GNU AND ${CMAKE_SYSTEM_NAME} MATCHES "Windows")
          target_link_libraries(cpp-netlib-http-${test} ws2_32)
        endif()
	if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	  target_link_libraries(cpp-netlib-http-${test} rt)
	endif()
        set_target_properties(cpp-netlib-http-${test}
            PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CPP-NETLIB_BINARY_DIR}/tests)
        add_test(cpp-netlib-http-${test}
            ${CPP-NETLIB_BINARY_DIR}/tests/cpp-netlib-http-${test})
    endforeach (test)

    set(SERVER_API_TESTS server_constructor_test
       server_header_parser_test)
    foreach (test ${SERVER_API_TESTS})
        add_executable(cpp-netlib-http-${test} ${test}.cpp)
        add_dependencies(cpp-netlib-http-${test} cppnetlib-server-parsers)
	target_link_libraries(cpp-netlib-http-${test}
	  ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} cppnetlib-server-parsers gtest_main)
        if (OPENSSL_FOUND)
            target_link_libraries(cpp-netlib-http-${test} ${OPENSSL_LIBRARIES})
        endif()
	if (${CMAKE_CXX_COMPILER_ID} MATCHES GNU AND ${CMAKE_SYSTEM_NAME} MATCHES "Windows")
          target_link_libraries(cpp-netlib-http-${test} ws2_32 wsock32)
        endif()
	if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	  target_link_libraries(cpp-netlib-http-${test} rt)
	endif()
        set_target_properties(cpp-netlib-http-${test}
            PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CPP-NETLIB_BINARY_DIR}/tests)
        add_test(cpp-netlib-http-${test}
            ${CPP-NETLIB_BINARY_DIR}/tests/cpp-netlib-http-${test})
    endforeach (test)

    # Add the server start/stop concurrency test
    add_executable(cpp-netlib-http-server_async_run_stop_concurrency
      server_async_run_stop_concurrency.cpp)
    add_dependencies(cpp-netlib-http-server_async_run_stop_concurrency
      cppnetlib-server-parsers)
    target_link_libraries(cpp-netlib-http-server_async_run_stop_concurrency
      ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} cppnetlib-server-parsers)
    if (OPENSSL_FOUND)
      target_link_libraries(cpp-netlib-http-server_async_run_stop_concurrency
	${OPENSSL_LIBRARIES})
    endif()
    if (${CMAKE_CXX_COMPILER_ID} MATCHES GNU
       	AND ${CMAKE_SYSTEM_NAME} MATCHES "Windows")
      target_link_libraries(cpp-netlib-http-server_async_run_stop_concurrency
	ws2_32 wsock32)
    endif()
    if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
      target_link_libraries(cpp-netlib-http-server_async_run_stop_concurrency rt)
    endif()
    set_target_properties(cpp-netlib-http-server_async_run_stop_concurrency
      PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CPP-NETLIB_BINARY_DIR}/tests)
    add_test(cpp-netlib-http-server_async_run_stop_concurrency
      ${CPP-NETLIB_BINARY_DIR}/tests/cpp-netlib-http-server_async_run_stop_concurrency)
endif()
