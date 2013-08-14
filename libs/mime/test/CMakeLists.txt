include_directories(${CPP-NETLIB_SOURCE_DIR})
find_package ( Boost 1.41.0 COMPONENTS unit_test_framework )
file ( COPY TestMessages DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )

option( CPP-NETLIB_BUILD_SHARED_LIBS "Build cpp-netlib as shared libraries." OFF )

if(CPP-NETLIB_BUILD_SHARED_LIBS OR BUILD_SHARED_LIBS)
  message ("Linking boost testing libs dynamically...")
  set(Boost_USE_STATIC_LIBS OFF)
  set(CPP-NETLIB_BUILD_SHARED_LIBS ON)
  add_definitions(-DBOOST_TEST_DYN_LINK)
else()
  set(Boost_USE_STATIC_LIBS ON)
  set(CPP-NETLIB_BUILD_SHARED_LIBS OFF)
  set(BUILD_SHARED_LIBS OFF)
endif()

set ( Boost_USE_MULTITHREADED ON )

if ( Boost_FOUND )
    add_executable ( mime-roundtrip mime-roundtrip.cpp )
    target_link_libraries ( mime-roundtrip ${Boost_LIBRARIES} )
    add_test ( mime-roundtrip mime-roundtrip )
endif ()

