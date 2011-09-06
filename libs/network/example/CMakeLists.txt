#            Copyright (c) Dean Michael Berris 2010.
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE_1_0.txt or copy at
#          http://www.boost.org/LICENSE_1_0.txt)

include_directories(${CPP-NETLIB_SOURCE_DIR})
find_package( Boost 1.41.0 COMPONENTS program_options system regex date_time thread filesystem )
find_package( OpenSSL )
find_package( Threads )

if (Boost_FOUND)
    set(Boost_USE_STATIC_LIBS ON)
    set(Boost_USE_MULTITHREADED ON)
endif (Boost_FOUND)

if (OPENSSL_FOUND)
  include_directories(${OPENSSL_INCLUDE_DIR})
endif (OPENSSL_FOUND)

if (Boost_FOUND)
    add_executable(http_client http_client.cpp)
    add_dependencies(http_client cppnetlib-uri-parsers)
    target_link_libraries(http_client ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} cppnetlib-uri-parsers)
    set_target_properties(http_client PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CPP-NETLIB_BINARY_DIR}/example)
    if (OPENSSL_FOUND)
      target_link_libraries(http_client ${OPENSSL_LIBRARIES})
    endif (OPENSSL_FOUND)

    add_executable(simple_wget simple_wget.cpp)
    add_dependencies(simple_wget cppnetlib-uri-parsers)
    target_link_libraries(simple_wget ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} cppnetlib-uri-parsers)
    set_target_properties(simple_wget PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CPP-NETLIB_BINARY_DIR}/example)
    if (OPENSSL_FOUND)
      target_link_libraries(simple_wget ${OPENSSL_LIBRARIES})
    endif (OPENSSL_FOUND)

    add_executable(hello_world_server http/hello_world_server.cpp)
    target_link_libraries(hello_world_server ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} )
    set_target_properties(hello_world_server PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CPP-NETLIB_BINARY_DIR}/example)

    if (UNIX)
      add_executable(fileserver http/fileserver.cpp)
      target_link_libraries(fileserver ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} cppnetlib-server-parsers)
      set_target_properties(fileserver PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CPP-NETLIB_BINARY_DIR}/example)
    endif (UNIX)

    add_executable(uri uri.cpp)
    add_dependencies(uri cppnetlib-uri-parsers)
    target_link_libraries(uri cppnetlib-uri-parsers)
    set_target_properties(uri PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CPP-NETLIB_BINARY_DIR}/example)

    add_executable(uri_builder uri_builder.cpp)
    add_dependencies(uri_builder cppnetlib-uri-parsers)
    target_link_libraries(uri_builder cppnetlib-uri-parsers)
    set_target_properties(uri_builder PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CPP-NETLIB_BINARY_DIR}/example)

endif()
