# Copyright (c) Glyn Matthews 2012.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)


if( CPP-NETLIB_ALWAYS_LOGGING )
  add_definitions( /D NETWORK_ENABLE_LOGGING )
endif()

if( NOT CPP-NETLIB_DISABLE_LOGGING )
  add_subdirectory(src)

  # this library name is defined only if we created the target
  # if not then it will be empty
  set( CPP-NETLIB_LOGGING_LIB cppnetlib-logging )

  if(CPP-NETLIB_BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
  endif(CPP-NETLIB_BUILD_TESTS)

endif()
