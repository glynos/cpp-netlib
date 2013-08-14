include_directories(${CPP-NETLIB_SOURCE_DIR})
find_package ( Boost 1.41.0 COMPONENTS unit_test_framework )
file ( COPY TestMessages DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )

option (BOOST_DYN_LINK
  "Build the project using dynamic linking for the boost test libs"
  OFF)
if (BOOST_DYN_LINK)
  message ("Do not try to link boost test libs dynamically for this test...")
  remove_definitions(-DBOOST_TEST_DYN_LINK)
elseif (BOOST_DYN_LINK)
  set(Boost_USE_STATIC_LIBS ON)
endif(BOOST_DYN_LINK)

set ( Boost_USE_MULTITHREADED ON )

if ( Boost_FOUND )
    add_executable ( mime-roundtrip mime-roundtrip.cpp )
    target_link_libraries ( mime-roundtrip ${Boost_LIBRARIES} )
    add_test ( mime-roundtrip mime-roundtrip )
endif ()

