#            Copyright (c) Dean Michael Berris 2010.
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE_1_0.txt or copy at
#          http://www.boost.org/LICENSE_1_0.txt)

include_directories(${CPP-NETLIB_SOURCE_DIR})
include_directories(${CPP-NETLIB_SOURCE_DIR}/deps/cxxopts/src)
if (OPENSSL_FOUND)
  include_directories(${OPENSSL_INCLUDE_DIR})
endif (OPENSSL_FOUND)

add_executable(http_client http_client.cpp)
add_executable(simple_wget simple_wget.cpp)
add_executable(atom_reader atom/atom.cpp atom/main.cpp)
add_executable(rss_reader rss/rss.cpp rss/main.cpp)
add_executable(hello_world_server http/hello_world_server.cpp)
add_executable(hello_world_client http/hello_world_client.cpp)
add_executable(hello_world_async_server_with_work_queue http/hello_world_async_server_with_work_queue.cpp)
add_executable(trivial_google trivial_google.cpp)

if (UNIX)
  add_executable(fileserver http/fileserver.cpp)
endif (UNIX)
add_dependencies(http_client cppnetlib-uri cppnetlib-client-connections)
add_dependencies(simple_wget cppnetlib-uri cppnetlib-client-connections)
add_dependencies(atom_reader cppnetlib-uri cppnetlib-client-connections)
add_dependencies(rss_reader cppnetlib-uri cppnetlib-client-connections)
add_dependencies(trivial_google cppnetlib-uri cppnetlib-client-connections)

target_link_libraries(http_client
    ${CMAKE_THREAD_LIBS_INIT}
    cppnetlib-uri
    cppnetlib-client-connections)

target_link_libraries(simple_wget
    ${CMAKE_THREAD_LIBS_INIT}
    cppnetlib-uri
    cppnetlib-client-connections)

target_link_libraries(atom_reader
    ${CMAKE_THREAD_LIBS_INIT}
    cppnetlib-uri
    cppnetlib-client-connections)

target_link_libraries(rss_reader
    ${CMAKE_THREAD_LIBS_INIT}
    cppnetlib-uri
    cppnetlib-client-connections)

target_link_libraries(trivial_google
  ${CMAKE_THREAD_LIBS_INIT}
  cppnetlib-uri
  cppnetlib-client-connections)

target_link_libraries(hello_world_server
    ${CMAKE_THREAD_LIBS_INIT}
    cppnetlib-server-parsers)

target_link_libraries(hello_world_client
    ${CMAKE_THREAD_LIBS_INIT}
    cppnetlib-uri
    cppnetlib-client-connections)

target_link_libraries(hello_world_async_server_with_work_queue
    ${CMAKE_THREAD_LIBS_INIT}
    cppnetlib-uri
    cppnetlib-client-connections
    cppnetlib-server-parsers)

if (OPENSSL_FOUND)
    add_executable(ssl_server http/ssl/ssl_server.cpp)
    add_dependencies(ssl_server cppnetlib-uri cppnetlib-client-connections)
    target_link_libraries(ssl_server
        ${CMAKE_THREAD_LIBS_INIT}
        cppnetlib-server-parsers
        cppnetlib-uri
        cppnetlib-client-connections)
endif (OPENSSL_FOUND)


if (OPENSSL_FOUND)
  target_link_libraries(http_client ${OPENSSL_LIBRARIES})
  target_link_libraries(simple_wget ${OPENSSL_LIBRARIES})
  target_link_libraries(atom_reader ${OPENSSL_LIBRARIES})
  target_link_libraries(rss_reader ${OPENSSL_LIBRARIES})
  target_link_libraries(hello_world_server ${OPENSSL_LIBRARIES})
  target_link_libraries(hello_world_client ${OPENSSL_LIBRARIES})
  target_link_libraries(hello_world_async_server_with_work_queue ${OPENSSL_LIBRARIES})
  target_link_libraries(ssl_server ${OPENSSL_LIBRARIES})
  target_link_libraries(trivial_google ${OPENSSL_LIBRARIES})
endif (OPENSSL_FOUND)

if (${CMAKE_CXX_COMPILER_ID} MATCHES GNU AND ${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  target_link_libraries(http_client ws2_32)
  target_link_libraries(simple_wget ws2_32)
  target_link_libraries(atom_reader ws2_32)
  target_link_libraries(rss_reader ws2_32)
  target_link_libraries(hello_world_server ws2_32 wsock32)
  target_link_libraries(hello_world_client ws2_32)
  target_link_libraries(hello_world_async_server_with_work_queue ws2_32 wsock32)
  target_link_libraries(ssl_server ws2_32 wsock32)
  target_link_libraries(trivial_google ws2_32)
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  target_link_libraries(http_client rt)
  target_link_libraries(simple_wget rt)
  target_link_libraries(atom_reader rt)
  target_link_libraries(rss_reader rt)
  target_link_libraries(hello_world_server rt)
  target_link_libraries(hello_world_client rt)
  target_link_libraries(hello_world_async_server_with_work_queue rt)
  target_link_libraries(trivial_google rt)
  if (OPENSSL_FOUND)
    target_link_libraries(ssl_server rt)
  endif (OPENSSL_FOUND)
endif()

if (UNIX)
  target_link_libraries(fileserver
    ${CMAKE_THREAD_LIBS_INIT}
    cppnetlib-server-parsers)
  if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    target_link_libraries(fileserver rt)
  endif()
  if (OPENSSL_FOUND)
    target_link_libraries(fileserver ${OPENSSL_LIBRARIES})
  endif(OPENSSL_FOUND)
endif (UNIX)

set_target_properties(http_client PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CPP-NETLIB_BINARY_DIR}/example)
set_target_properties(simple_wget PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CPP-NETLIB_BINARY_DIR}/example)
set_target_properties(atom_reader PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CPP-NETLIB_BINARY_DIR}/example)
set_target_properties(rss_reader PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CPP-NETLIB_BINARY_DIR}/example)
set_target_properties(trivial_google PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CPP-NETLIB_BINARY_DIR}/example)
set_target_properties(hello_world_server PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CPP-NETLIB_BINARY_DIR}/example)
set_target_properties(hello_world_client PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CPP-NETLIB_BINARY_DIR}/example)
set_target_properties(hello_world_async_server_with_work_queue PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CPP-NETLIB_BINARY_DIR}/example)
if (OPENSSL_FOUND)
  set_target_properties(ssl_server PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CPP-NETLIB_BINARY_DIR}/example)
endif (OPENSSL_FOUND)
  
if (UNIX)
  set_target_properties(fileserver PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CPP-NETLIB_BINARY_DIR}/example)
endif (UNIX)
