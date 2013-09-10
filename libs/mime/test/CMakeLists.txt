include_directories(${CPP-NETLIB_SOURCE_DIR})
file ( COPY TestMessages DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )

if ( Boost_FOUND )
    add_executable ( mime-roundtrip mime-roundtrip.cpp )
    target_link_libraries ( mime-roundtrip ${Boost_LIBRARIES}
      ${CMAKE_THREAD_LIBS_INIT})
    add_test ( mime-roundtrip mime-roundtrip )
endif ()

