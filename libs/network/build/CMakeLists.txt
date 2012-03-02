include_directories(${CPP-NETLIB_SOURCE_DIR})
find_package( Boost 1.45.0 COMPONENTS unit_test_framework system regex thread filesystem )

add_library(cppnetlib-uri STATIC ${CPP-NETLIB_SOURCE_DIR}/libs/network/src/parse_uri_impl.cpp)
add_library(cppnetlib-server-parsers STATIC ${CPP-NETLIB_SOURCE_DIR}/libs/network/src/server_request_parsers_impl.cpp)
add_library(cppnetlib-message
    STATIC
    ${CPP-NETLIB_SOURCE_DIR}/libs/network/src/message/message.cpp)
add_library(cppnetlib-message-directives
    STATIC
    ${CPP-NETLIB_SOURCE_DIR}/libs/network/src/message/directives.cpp)
add_library(cppnetlib-http-message
    STATIC
    ${CPP-NETLIB_SOURCE_DIR}/libs/network/src/http/request.cpp
    ${CPP-NETLIB_SOURCE_DIR}/libs/network/src/http/response.cpp)
add_library(cppnetlib-http-client-connections
    STATIC
    ${CPP-NETLIB_SOURCE_DIR}/libs/network/src/http/client_connections.cpp
    ${CPP-NETLIB_SOURCE_DIR}/libs/network/src/http/simple_connection_manager.cpp
    ${CPP-NETLIB_SOURCE_DIR}/libs/network/src/http/simple_connection_factory.cpp
    ${CPP-NETLIB_SOURCE_DIR}/libs/network/src/http/connection_delegate_factory.cpp)
add_library(cppnetlib-http-client
    STATIC
    ${CPP-NETLIB_SOURCE_DIR}/libs/network/src/http/client.cpp)
