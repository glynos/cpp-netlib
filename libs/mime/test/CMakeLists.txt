include_directories(${CPP-NETLIB_SOURCE_DIR})

if ( Boost_FOUND )
    add_executable ( mime-roundtrip mime-roundtrip.cpp )
    target_link_libraries ( mime-roundtrip )
    add_test ( mime-roundtrip mime-roundtrip )
endif ()

