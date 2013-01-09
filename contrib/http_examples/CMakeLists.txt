#            Copyright (c) Dean Michael Berris 2010.
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE_1_0.txt or copy at
#          http://www.boost.org/LICENSE_1_0.txt)

include_directories(
  ${CPP-NETLIB_SOURCE_DIR}/uri/src
  ${CPP-NETLIB_SOURCE_DIR}/message/src
  ${CPP-NETLIB_SOURCE_DIR}/logging/src
  ${CPP-NETLIB_SOURCE_DIR}/http/src
  ${CPP-NETLIB_SOURCE_DIR})
if (OPENSSL_FOUND)
  include_directories(${OPENSSL_INCLUDE_DIR})
endif (OPENSSL_FOUND)

if( NOT CPP-NETLIB_DISABLE_LOGGING )
  set( CPP-NETLIB_LOGGING_LIB cppnetlib-logging )
endif()

add_executable(simple_wget simple_wget.cpp)
add_executable(atom_reader atom/atom.cpp atom/main.cpp)
add_executable(rss_reader rss/rss.cpp rss/main.cpp)
#add_executable(twitter_search twitter/search.cpp)
#add_executable(hello_world_server http/hello_world_server.cpp)
add_executable(hello_world_client http/hello_world_client.cpp)
#if (UNIX)
#  add_executable(fileserver http/fileserver.cpp)
#endif (UNIX)
set(BOOST_CLIENT_LIBS
  ${Boost_DATE_TIME_LIBRARY}
  ${Boost_SYSTEM_LIBRARY}
  ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_PROGRAM_OPTIONS_LIBRARY}
  ${Boost_REGEX_LIBRARY}
  )
set(BOOST_SERVER_LIBS
  ${Boost_DATE_TIME_LIBRARY}
  ${Boost_SYSTEM_LIBRARY}
  ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_PROGRAM_OPTIONS_LIBRARY}
  )

target_link_libraries(simple_wget
    ${BOOST_CLIENT_LIBS}
    ${CMAKE_THREAD_LIBS_INIT}
    cppnetlib-uri
    cppnetlib-message
    cppnetlib-message-directives
    cppnetlib-message-wrappers
    cppnetlib-http-message-wrappers
    cppnetlib-http-message
    cppnetlib-constants
    cppnetlib-http-client
    cppnetlib-http-client-connections
    ${CPP-NETLIB_LOGGING_LIB})

target_link_libraries(atom_reader
    ${BOOST_CLIENT_LIBS}
    ${CMAKE_THREAD_LIBS_INIT}
    cppnetlib-uri
    cppnetlib-message
    cppnetlib-message-directives
    cppnetlib-message-wrappers
    cppnetlib-http-message
    cppnetlib-http-message-wrappers
    cppnetlib-constants
    cppnetlib-http-client
    cppnetlib-http-client-connections
    ${CPP-NETLIB_LOGGING_LIB})

target_link_libraries(rss_reader
    ${BOOST_CLIENT_LIBS}
    ${CMAKE_THREAD_LIBS_INIT}
    cppnetlib-uri
    cppnetlib-message
    cppnetlib-message-directives
    cppnetlib-message-wrappers
    cppnetlib-http-message
    cppnetlib-constants
    cppnetlib-http-client
    cppnetlib-http-client-connections
    ${CPP-NETLIB_LOGGING_LIB})

#target_link_libraries(twitter_search
#    ${BOOST_CLIENT_LIBS}
#    ${CMAKE_THREAD_LIBS_INIT}
#    cppnetlib-uri
#    cppnetlib-message
#    cppnetlib-message-directives
#    cppnetlib-message-wrappers
#    cppnetlib-http-message-wrappers
#    cppnetlib-http-message
#    cppnetlib-constants
#    cppnetlib-http-client
#    cppnetlib-http-client-connections
#    ${CPP-NETLIB_LOGGING_LIB})

#target_link_libraries(hello_world_server
#    ${BOOST_SERVER_LIBS}
#    ${CMAKE_THREAD_LIBS_INIT}
#    cppnetlib-http-server-parsers
#    cppnetlib-http-server
#    ${CPP-NETLIB_LOGGING_LIB})

target_link_libraries(hello_world_client
    ${BOOST_CLIENT_LIBS}
    ${CMAKE_THREAD_LIBS_INIT}
    cppnetlib-uri
    cppnetlib-message
    cppnetlib-message-directives
    cppnetlib-message-wrappers
    cppnetlib-http-message-wrappers
    cppnetlib-http-message
    cppnetlib-constants
    cppnetlib-http-client
    cppnetlib-http-client-connections
    ${CPP-NETLIB_LOGGING_LIB})

if (OPENSSL_FOUND)
  target_link_libraries(simple_wget ${OPENSSL_LIBRARIES})
  target_link_libraries(atom_reader ${OPENSSL_LIBRARIES})
  target_link_libraries(rss_reader ${OPENSSL_LIBRARIES})
  #target_link_libraries(twitter_search ${OPENSSL_LIBRARIES})
  #target_link_libraries(hello_world_server ${OPENSSL_LIBRARIES})
  target_link_libraries(hello_world_client ${OPENSSL_LIBRARIES})
endif (OPENSSL_FOUND)

#if (UNIX)
#  target_link_libraries(fileserver
#    ${BOOST_SERVER_LIBS}
#    ${Boost_FILESYSTEM_LIBRARY}
#    ${CMAKE_THREAD_LIBS_INIT}
#    cppnetlib-server-parsers)
#endif (UNIX)

set_target_properties(simple_wget PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CPP-NETLIB_BINARY_DIR}/example)
set_target_properties(atom_reader PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CPP-NETLIB_BINARY_DIR}/example)
set_target_properties(rss_reader PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CPP-NETLIB_BINARY_DIR}/example)
#set_target_properties(twitter_search PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CPP-NETLIB_BINARY_DIR}/example)
#set_target_properties(hello_world_server PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CPP-NETLIB_BINARY_DIR}/example)
set_target_properties(hello_world_client PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CPP-NETLIB_BINARY_DIR}/example)
#if (UNIX)
#  set_target_properties(fileserver PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CPP-NETLIB_BINARY_DIR}/example)
#endif (UNIX)
