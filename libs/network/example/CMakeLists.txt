#            Copyright (c) Dean Michael Berris 2010.
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE_1_0.txt or copy at
#          http://www.boost.org/LICENSE_1_0.txt)

include_directories(${CPP-NETLIB_SOURCE_DIR})
include_directories(${OPENSSL_INCLUDE_DIR})

add_executable(http_client http_client.cpp)
add_executable(simple_wget simple_wget.cpp)
add_executable(hello_world_server http/hello_world_server.cpp)
add_executable(fileserver http/fileserver.cpp)
add_executable(uri uri.cpp)
add_dependencies(http_client cppnetlib-uri-parsers cppnetlib-client-connections)
add_dependencies(simple_wget cppnetlib-uri-parsers cppnetlib-client-connections)
add_dependencies(uri cppnetlib-uri-parsers)
set(BOOST_CLIENT_LIBS
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
    ${Boost_THREAD_LIBRARY}
    ${Boost_SYSTEM_LIBRARY})

set(BOOST_SERVER_LIBS
    ${Boost_THREAD_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_PROGRAM_OPTIONS_LIBRARY})

target_link_libraries(http_client
    ${BOOST_CLIENT_LIBS}
    ${CMAKE_THREAD_LIBS_INIT}
    ${OPENSSL_LIBRARIES}
    cppnetlib-uri-parsers
    cppnetlib-client-connections)

target_link_libraries(simple_wget
    ${BOOST_CLIENT_LIBS}
    ${CMAKE_THREAD_LIBS_INIT}
    ${OPENSSL_LIBRARIES}
    cppnetlib-uri-parsers
    cppnetlib-client-connections)

target_link_libraries(hello_world_server
    ${BOOST_SERVER_LIBS}
    ${CMAKE_THREAD_LIBS_INIT}
    ${OPENSSL_LIBRARIES})

target_link_libraries(fileserver
    ${BOOST_SERVER_LIBS}
    ${Boost_FILESYSTEM_LIBRARY}
    ${CMAKE_THREAD_LIBS_INIT}
    cppnetlib-server-parsers)

target_link_libraries(uri cppnetlib-uri-parsers)
set_target_properties(http_client PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CPP-NETLIB_BINARY_DIR}/example)
set_target_properties(simple_wget PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CPP-NETLIB_BINARY_DIR}/example)
set_target_properties(hello_world_server PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CPP-NETLIB_BINARY_DIR}/example)
set_target_properties(fileserver PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CPP-NETLIB_BINARY_DIR}/example)
set_target_properties(uri PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CPP-NETLIB_BINARY_DIR}/example)
