# Copyright (c) Glyn Matthews 2011, 2012.
# Copyright 2011 Dean Michael Berris (dberris@google.com)
# Copyright 2011 Google, Inc.
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE_1_0.txt or copy at
#          http://www.boost.org/LICENSE_1_0.txt)


include_directories(${CPP-NETLIB_SOURCE_DIR}/include)
include_directories(${CPP-NETLIB_SOURCE_DIR})
if (OPENSSL_FOUND)
include_directories(${OPENSSL_INCLUDE_DIR})
endif()

if (${CMAKE_CXX_COMPILER_ID} MATCHES GNU)
  if (HAVE_STD11)
    set(CPP-NETLIB_CXXFLAGS "-Wall -std=c++11")
  elseif (HAVE_STD0X)
    set(CPP-NETLIB_CXXFLAGS "-Wall -std=c++0x")
  endif()
endif()

set(CPP-NETLIB_URI_SRCS uri/uri.cpp uri/schemes.cpp uri/normalize.cpp)
add_library(cppnetlib-uri ${CPP-NETLIB_URI_SRCS})
foreach (src_file ${CPP-NETLIB_URI_SRCS})
if (${CMAKE_CXX_COMPILER_ID} MATCHES GNU)
    set_source_files_properties(${src_file}
        PROPERTIES COMPILE_FLAGS ${CPP-NETLIB_CXXFLAGS})
endif()
endforeach(src_file)

set(CPP-NETLIB_MESSAGE_SRCS message/message.cpp)
add_library(cppnetlib-message ${CPP-NETLIB_MESSAGE_SRCS})
add_dependencies(cppnetlib-message cppnetlib-uri)
target_link_libraries(cppnetlib-message cppnetlib-uri)
foreach (src_file ${CPP-NETLIB_MESSAGE_SRCS})
if (${CMAKE_CXX_COMPILER_ID} MATCHES GNU)
    set_source_files_properties(${src_file}
        PROPERTIES COMPILE_FLAGS ${CPP-NETLIB_CXXFLAGS})
endif()
endforeach(src_file)

set(CPP-NETLIB_MESSAGE_DIRECTIVES_SRCS message/directives.cpp)
add_library(cppnetlib-message-directives ${CPP-NETLIB_MESSAGE_DIRECTIVES_SRCS})
foreach (src_file ${CPP-NETLIB_MESSAGE_DIRECTIVES_SRCS})
if (${CMAKE_CXX_COMPILER_ID} MATCHES GNU)
    set_source_files_properties(${src_file}
        PROPERTIES COMPILE_FLAGS ${CPP-NETLIB_CXXFLAGS})
endif()
endforeach(src_file)

set(CPP-NETLIB_MESSAGE_WRAPPERS_SRCS message/wrappers.cpp)
add_library(cppnetlib-message-wrappers ${CPP-NETLIB_MESSAGE_WRAPPERS_SRCS})
foreach (src_file ${CPP-NETLIB_MESSAGE_WRAPPERS_SRCS})
if (${CMAKE_CXX_COMPILER_ID} MATCHES GNU)
    set_source_files_properties(${src_file}
        PROPERTIES COMPILE_FLAGS ${CPP-NETLIB_CXXFLAGS})
endif()
endforeach(src_file)

set(CPP-NETLIB_HTTP_MESSAGE_SRCS http/request.cpp http/response.cpp)
add_library(cppnetlib-http-message ${CPP-NETLIB_HTTP_MESSAGE_SRCS})
add_dependencies(cppnetlib-http-message
#   ${Boost_LIBRARIES}
  cppnetlib-message)
target_link_libraries(cppnetlib-http-message
  ${Boost_LIBRARIES}
  cppnetlib-message)
foreach (src_file ${CPP-NETLIB_HTTP_MESSAGE_SRCS})
if (${CMAKE_CXX_COMPILER_ID} MATCHES GNU)
    set_source_files_properties(${src_file}
        PROPERTIES COMPILE_FLAGS ${CPP-NETLIB_CXXFLAGS})
endif()
endforeach(src_file)

set(CPP-NETLIB_HTTP_MESSAGE_WRAPPERS_SRCS http/message/wrappers.cpp)
add_library(cppnetlib-http-message-wrappers ${CPP-NETLIB_HTTP_MESSAGE_WRAPPERS_SRCS})
foreach (src_file ${CPP-NETLIB_HTTP_MESSAGE_WRAPPERS_SRCS})
if (${CMAKE_CXX_COMPILER_ID} MATCHES GNU)
    set_source_files_properties(${src_file}
        PROPERTIES COMPILE_FLAGS ${CPP-NETLIB_CXXFLAGS})
endif()
endforeach(src_file)

set(CPP-NETLIB_HTTP_SERVER_PARSERS_SRCS server_request_parsers_impl.cpp)
add_library(cppnetlib-http-server-parsers ${CPP-NETLIB_HTTP_SERVER_PARSERS_SRCS})
foreach (src_file ${CPP-NETLIB_HTTP_SERVER_PARSERS_SRCS})
if (${CMAKE_CXX_COMPILER_ID} MATCHES GNU)
    set_source_files_properties(${src_file}
        PROPERTIES COMPILE_FLAGS ${CPP-NETLIB_CXXFLAGS})
endif()
endforeach(src_file)

set(CPP-NETLIB_HTTP_SERVER_SRCS
    http/server_async_impl.cpp
    http/server_options.cpp
    http/server_socket_options_setter.cpp
    http/server_sync_impl.cpp
    )
add_library(cppnetlib-http-server ${CPP-NETLIB_HTTP_SERVER_SRCS})
add_dependencies(cppnetlib-http-server
  cppnetlib-constants
  cppnetlib-uri
  cppnetlib-message
  cppnetlib-message-wrappers
  cppnetlib-message-directives
  cppnetlib-http-message
  cppnetlib-http-message-wrappers
  cppnetlib-http-server-parsers
  cppnetlib-utils-thread_pool
  )
target_link_libraries(cppnetlib-http-server
  ${Boost_LIBRARIES}
  cppnetlib-constants
  cppnetlib-uri
  cppnetlib-message
  cppnetlib-message-wrappers
  cppnetlib-message-directives
  cppnetlib-http-message
  cppnetlib-http-message-wrappers
  cppnetlib-http-server-parsers
  cppnetlib-utils-thread_pool
  )
foreach (src_file ${CPP-NETLIB_HTTP_SERVER_SRCS})
if (${CMAKE_CXX_COMPILER_ID} MATCHES GNU)
    set_source_files_properties(${src_file}
        PROPERTIES COMPILE_FLAGS ${CPP-NETLIB_CXXFLAGS})
endif()
endforeach(src_file)

set(CPP-NETLIB_HTTP_CLIENT_CONNECTIONS_SRCS
    http/client_connections.cpp
    http/simple_connection_manager.cpp
    http/simple_connection_factory.cpp
    http/connection_delegate_factory.cpp
    http/client_resolver_delegate.cpp
    http/client_resolver_delegate_factory.cpp
    http/client_connection_delegates.cpp
    http/client_connection_factory.cpp
    http/client_async_resolver.cpp
    http/client_connection_normal.cpp)
add_library(cppnetlib-http-client-connections ${CPP-NETLIB_HTTP_CLIENT_CONNECTIONS_SRCS})
foreach (src_file ${CPP-NETLIB_HTTP_CLIENT_CONNECTIONS_SRCS})
if (${CMAKE_CXX_COMPILER_ID} MATCHES GNU)
    set_source_files_properties(${src_file}
        PROPERTIES COMPILE_FLAGS ${CPP-NETLIB_CXXFLAGS})
endif()
endforeach(src_file)

set(CPP-NETLIB_HTTP_CLIENT_SRCS
    http/client.cpp)
add_library(cppnetlib-http-client ${CPP-NETLIB_HTTP_CLIENT_SRCS})
add_dependencies(cppnetlib-http-client
  cppnetlib-constants
  cppnetlib-uri
  cppnetlib-message
  cppnetlib-message-wrappers
  cppnetlib-message-directives
  cppnetlib-http-message
  cppnetlib-http-message-wrappers
  cppnetlib-http-client-connections
  )
target_link_libraries(cppnetlib-http-client
  ${Boost_LIBRARIES}
  cppnetlib-constants
  cppnetlib-uri
  cppnetlib-message
  cppnetlib-message-wrappers
  cppnetlib-message-directives
  cppnetlib-http-message
  cppnetlib-http-message-wrappers
  cppnetlib-http-client-connections
  )
foreach (src_file ${CPP-NETLIB_HTTP_CLIENT_SRCS})
if (${CMAKE_CXX_COMPILER_ID} MATCHES GNU)
    set_source_files_properties(${src_file}
        PROPERTIES COMPILE_FLAGS ${CPP-NETLIB_CXXFLAGS})
endif()
endforeach(src_file)

set(CPP-NETLIB_UTILS_THREAD_POOL_SRCS utils/thread_pool.cpp)
add_library(cppnetlib-utils-thread_pool ${CPP-NETLIB_UTILS_THREAD_POOL_SRCS})
foreach (src_file ${CPP-NETLIB_UTILS_THREAD_POOL_SRCS})
if (${CMAKE_CXX_COMPILER_ID} MATCHES GNU)
    set_source_files_properties(${src_file}
        PROPERTIES COMPILE_FLAGS ${CPP-NETLIB_CXXFLAGS})
endif()
endforeach(src_file)

set(CPP-NETLIB_CONSTANTS_SRCS constants.cpp)
add_library(cppnetlib-constants ${CPP-NETLIB_CONSTANTS_SRCS})
foreach (src_file ${CPP-NETLIB_CONSTANTS_SRCS})
if (${CMAKE_CXX_COMPILER_ID} MATCHES GNU)
    set_source_files_properties(${src_file}
        PROPERTIES COMPILE_FLAGS ${CPP-NETLIB_CXXFLAGS})
endif()
endforeach(src_file)
