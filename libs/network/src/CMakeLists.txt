# Copyright (c) Glyn Matthews 2011.
# Copyright 2011 Dean Michael Berris (dberris@google.com)
# Copyright 2011 Google, Inc.
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE_1_0.txt or copy at
#          http://www.boost.org/LICENSE_1_0.txt)


include_directories(${CPP-NETLIB_SOURCE_DIR})
set(CPP-NETLIB_URI_SRCS uri/parse.cpp)
add_library(cppnetlib-uri ${CPP-NETLIB_URI_SRCS})

set(CPP-NETLIB_HTTP_SERVER_SRCS server_request_parsers_impl.cpp)
add_library(cppnetlib-server-parsers ${CPP-NETLIB_HTTP_SERVER_SRCS})

set(CPP-NETLIB_HTTP_CLIENT_SRCS client.cpp)
add_library(cppnetlib-client-connections ${CPP-NETLIB_HTTP_CLIENT_SRCS})

set(CPP-NETLIB_MESSAGE_SRCS message/message.cpp)
add_library(cppnetlib-message ${CPP-NETLIB_MESSAGE_SRCS})

set(CPP-NETLIB_MESSAGE_DIRECTIVES_SRCS message/directives.cpp)
add_library(cppnetlib-message-directives ${CPP-NETLIB_MESSAGE_DIRECTIVES_SRCS})

set(CPP-NETLIB_MESSAGE_WRAPPERS_SRCS message/wrappers.cpp)
add_library(cppnetlib-message-wrappers ${CPP-NETLIB_MESSAGE_WRAPPERS_SRCS})

set(CPP-NETLIB_HTTP_MESSAGE_SRCS http/request.cpp http/response.cpp)
add_library(cppnetlib-http-message ${CPP-NETLIB_HTTP_MESSAGE_SRCS})

set(CPP-NETLIB_HTTP_CLIENT_CONNECTIONS_SRCS
    http/client_connections.cpp
    http/simple_connection_manager.cpp
    http/simple_connection_factory.cpp
    http/connection_delegate_factory.cpp)
add_library(cppnetlib-http-client-connections ${CPP-NETLIB_HTTP_CLIENT_CONNECTIONS_SRCS})

set(CPP-NETLIB_UTILS_THREAD_POOL_SRCS utils/thread_pool.cpp)
add_library(cppnetlib-utils-thread_pool ${CPP-NETLIB_UTILS_THREAD_POOL_SRCS})
