#            Copyright (c) Glyn Matthews 2011.
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE_1_0.txt or copy at
#          http://www.boost.org/LICENSE_1_0.txt)


# boost_network_uri
include_directories(${CPP-NETLIB_SOURCE_DIR})

if (OPENSSL_FOUND)
    add_definitions(-DBOOST_NETWORK_ENABLE_HTTPS)
endif()

set(CPP-NETLIB_URI_SRCS uri/parse.cpp)
add_library(cppnetlib-uri-parsers ${CPP-NETLIB_URI_SRCS})

set(CPP-NETLIB_HTTP_SERVER_SRCS server_request_parsers_impl.cpp)
add_library(cppnetlib-server-parsers ${CPP-NETLIB_HTTP_SERVER_SRCS})
