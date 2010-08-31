#            Copyright (c) Dean Michael Berris 2010.
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE_1_0.txt or copy at
#          http://www.boost.org/LICENSE_1_0.txt)

include_directories(${CPP-NETLIB_SOURCE_DIR})
find_package( Boost 1.41.0 COMPONENTS unit_test_framework system regex date_time thread filesystem )
find_package( OpenSSL )
include_directories(${OPENSSL_INCLUDE_DIR})
find_package( Threads )
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)

if (Boost_FOUND)
    add_executable(http_client http_client.cpp)
    target_link_libraries(http_client ${Boost_SYSTEM_LIBRARY} ${Boost_REGEX_LIBRARY} ${Boost_DATE_TIME_LIBRARY} ${Boost_THREAD_LIBRARY} ${CMAKE_THREAD_LIBS_INIT} ${OPENSSL_LIBRARIES} )
    set_target_properties(http_client PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../../../build/example)
endif()
