# Copyright (c) Glyn Matthews 2011.
# Copyright 2011 Dean Michael Berris (dberris@google.com)
# Copyright 2011 Google, Inc.
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE_1_0.txt or copy at
#          http://www.boost.org/LICENSE_1_0.txt)


include_directories(${CPP-NETLIB_SOURCE_DIR})

set(CPP-NETLIB_URI_SRCS uri/parse.cpp)
add_library(cppnetlib-uri ${CPP-NETLIB_URI_SRCS})
foreach (src_file ${CPP-NETLIB_URI_SRCS})
if (${CMAKE_CXX_COMPILER_ID} MATCHES GNU)
    set_source_files_properties(${src_file}
        PROPERTIES COMPILE_FLAGS "-Wall -Werror")
endif()
endforeach(src_file)

set(CPP-NETLIB_HTTP_SERVER_SRCS server_request_parsers_impl.cpp)
add_library(cppnetlib-server-parsers ${CPP-NETLIB_HTTP_SERVER_SRCS})
foreach (src_file ${CPP-NETLIB_HTTP_SERVER_SRCS})
if (${CMAKE_CXX_COMPILER_ID} MATCHES GNU)
    set_source_files_properties(${src_file}
        PROPERTIES COMPILE_FLAGS "-Wall -Werror")
endif()
endforeach(src_file)

set(CPP-NETLIB_MESSAGE_SRCS message/message.cpp)
add_library(cppnetlib-message ${CPP-NETLIB_MESSAGE_SRCS})
foreach (src_file ${CPP-NETLIB_MESSAGE_SRCS})
if (${CMAKE_CXX_COMPILER_ID} MATCHES GNU)
    set_source_files_properties(${src_file}
        PROPERTIES COMPILE_FLAGS "-Wall -Werror")
endif()
endforeach(src_file)

set(CPP-NETLIB_MESSAGE_DIRECTIVES_SRCS message/directives.cpp)
add_library(cppnetlib-message-directives ${CPP-NETLIB_MESSAGE_DIRECTIVES_SRCS})
foreach (src_file ${CPP-NETLIB_MESSAGE_DIRECTIVES_SRCS})
if (${CMAKE_CXX_COMPILER_ID} MATCHES GNU)
    set_source_files_properties(${src_file}
        PROPERTIES COMPILE_FLAGS "-Wall -Werror")
endif()
endforeach(src_file)

set(CPP-NETLIB_MESSAGE_WRAPPERS_SRCS message/wrappers.cpp)
add_library(cppnetlib-message-wrappers ${CPP-NETLIB_MESSAGE_WRAPPERS_SRCS})
foreach (src_file ${CPP-NETLIB_MESSAGE_WRAPPERS_SRCS})
if (${CMAKE_CXX_COMPILER_ID} MATCHES GNU)
    set_source_files_properties(${src_file}
        PROPERTIES COMPILE_FLAGS "-Wall -Werror")
endif()
endforeach(src_file)

set(CPP-NETLIB_HTTP_MESSAGE_SRCS http/request.cpp http/response.cpp)
add_library(cppnetlib-http-message ${CPP-NETLIB_HTTP_MESSAGE_SRCS})
foreach (src_file ${CPP-NETLIB_HTTP_MESSAGE_SRCS})
if (${CMAKE_CXX_COMPILER_ID} MATCHES GNU)
    set_source_files_properties(${src_file}
        PROPERTIES COMPILE_FLAGS "-Wall -Werror")
endif()
endforeach(src_file)

set(CPP-NETLIB_HTTP_MESSAGE_WRAPPERS_SRCS http/message/wrappers.cpp)
add_library(cppnetlib-http-message-wrappers ${CPP-NETLIB_HTTP_MESSAGE_WRAPPERS_SRCS})
foreach (src_file ${CPP-NETLIB_HTTP_MESSAGE_WRAPPERS_SRCS})
if (${CMAKE_CXX_COMPILER_ID} MATCHES GNU)
    set_source_files_properties(${src_file}
        PROPERTIES COMPILE_FLAGS "-Wall -Werror")
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
        PROPERTIES COMPILE_FLAGS "-Wall -Werror")
endif()
endforeach(src_file)

set(CPP-NETLIB_HTTP_CLIENT_SRCS
    http/client.cpp)
add_library(cppnetlib-http-client ${CPP-NETLIB_HTTP_CLIENT_SRCS})
foreach (src_file ${CPP-NETLIB_HTTP_CLIENT_SRCS})
if (${CMAKE_CXX_COMPILER_ID} MATCHES GNU)
    set_source_files_properties(${src_file}
        PROPERTIES COMPILE_FLAGS "-Wall -Werror")
endif()
endforeach(src_file)

set(CPP-NETLIB_UTILS_THREAD_POOL_SRCS utils/thread_pool.cpp)
add_library(cppnetlib-utils-thread_pool ${CPP-NETLIB_UTILS_THREAD_POOL_SRCS})
foreach (src_file ${CPP-NETLIB_UTILS_THREAD_POOL_SRCS})
if (${CMAKE_CXX_COMPILER_ID} MATCHES GNU)
    set_source_files_properties(${src_file}
        PROPERTIES COMPILE_FLAGS "-Wall -Werror")
endif()
endforeach(src_file)

set(CPP-NETLIB_CONSTANTS_SRCS constants.cpp)
add_library(cppnetlib-constants ${CPP-NETLIB_CONSTANTS_SRCS})
foreach (src_file ${CPP-NETLIB_CONSTANTS_SRCS})
if (${CMAKE_CXX_COMPILER_ID} MATCHES GNU)
    set_source_files_properties(${src_file}
        PROPERTIES COMPILE_FLAGS "-Wall -Werror")
endif()
endforeach(src_file)
