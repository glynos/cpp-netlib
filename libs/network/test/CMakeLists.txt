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
    set_source_files_properties(
        http_incremental_parser.cpp hello_world.cpp
        http_1_0_test.cpp http_1_1_test.cpp
        http_localhost_tests.cpp https_localhost_tests.cpp
        message_test.cpp http_message_twst.cpp
        message_transform_test.cpp url_test.cpp
        PROPERTIES COMPILE_FLAGS "-Wall -Werror"
        )
    add_executable(cpp-netlib-http_incremental_parser http_incremental_parser.cpp)
    add_executable(cpp-netlib-hello_world hello_world.cpp)
    add_executable(cpp-netlib-http_1_0_test http_1_0_test.cpp)
    add_executable(cpp-netlib-http_1_1_test http_1_1_test.cpp)
    add_executable(cpp-netlib-http_localhost_tests http_localhost_tests.cpp)
    add_executable(cpp-netlib-https_localhost_tests https_localhost_tests.cpp)
    add_executable(cpp-netlib-message_test message_test.cpp)
    add_executable(cpp-netlib-http_message_test http_message_test.cpp)
    add_executable(cpp-netlib-message_transform_test message_transform_test.cpp)
    add_executable(cpp-netlib-url_test url_test.cpp)
    target_link_libraries(cpp-netlib-http_incremental_parser ${CMAKE_THREAD_LIBS_INIT} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
    target_link_libraries(cpp-netlib-hello_world ${Boost_SYSTEM_LIBRARY} ${Boost_REGEX_LIBRARY} ${Boost_DATE_TIME_LIBRARY} ${Boost_THREAD_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})
    target_link_libraries(cpp-netlib-http_1_0_test ${Boost_LIBRARIES}  ${CMAKE_THREAD_LIBS_INIT})
    target_link_libraries(cpp-netlib-http_1_1_test ${Boost_LIBRARIES}  ${CMAKE_THREAD_LIBS_INIT})
    target_link_libraries(cpp-netlib-http_message_test ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
    target_link_libraries(cpp-netlib-message_test ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
    target_link_libraries(cpp-netlib-message_transform_test ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
    target_link_libraries(cpp-netlib-http_localhost_tests ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
    target_link_libraries(cpp-netlib-https_localhost_tests ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
    target_link_libraries(cpp-netlib-url_test ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
    if (OPENSSL_FOUND)
        target_link_libraries(cpp-netlib-hello_world ${OPENSSL_LIBRARIES})
        target_link_libraries(cpp-netlib-http_1_0_test ${OPENSSL_LIBRARIES})
        target_link_libraries(cpp-netlib-http_1_1_test ${OPENSSL_LIBRARIES})
        target_link_libraries(cpp-netlib-http_message_test ${OPENSSL_LIBRARIES} )
        target_link_libraries(cpp-netlib-message_test ${OPENSSL_LIBRARIES} )
        target_link_libraries(cpp-netlib-message_transform_test ${OPENSSL_LIBRARIES} )
        target_link_libraries(cpp-netlib-http_localhost_tests ${OPENSSL_LIBRARIES} )
        target_link_libraries(cpp-netlib-https_localhost_tests ${OPENSSL_LIBRARIES} )
        target_link_libraries(cpp-netlib-url_test ${OPENSSL_LIBRARIES} )
    endif()
    set_target_properties(cpp-netlib-http_incremental_parser cpp-netlib-hello_world cpp-netlib-http_1_0_test cpp-netlib-http_1_1_test cpp-netlib-message_test cpp-netlib-http_message_test cpp-netlib-message_transform_test cpp-netlib-http_localhost_tests cpp-netlib-https_localhost_tests cpp-netlib-url_test PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../../../build/tests)
    add_test(cpp-netlib-http_incremental_parser ../../../build/tests/cpp-netlib-http_incremental_parser)
    add_test(cpp-netlib-hello_world python httplib_acceptance.py ../../../build/tests/cpp-netlib-hello_world ../../../build/tests/cpp-netlib-hello_world.passed)
    add_test(cpp-netlib-http_1_0_test ../../../build/tests/cpp-netlib-http_1_0_test)
    add_test(cpp-netlib-http_1_1_test ../../../build/tests/cpp-netlib-http_1_1_test)
    add_test(cpp-netlib-http_localhost_tests ../../../build/tests/cpp-netlib-http_localhost_tests)
    add_test(cpp-netlib-https_localhost_tests ../../../build/tests/cpp-netlib-https_localhost_tests)
    add_test(cpp-netlib-message_test ../../../build/tests/cpp-netlib-message_test)
    add_test(cpp-netlib-http_message_test ../../../build/tests/cpp-netlib-http_message_test)
    add_test(cpp-netlib-url_test ../../../build/tests/cpp-netlib-url_test)
endif()

