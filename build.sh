#!/bin/sh
set -e

mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=$CMAKE_BUILD_TYPE \
  -DBUILD_SHARED_LIBS=$BUILD_SHARED_LIBS \
  -DCPP-NETLIB_ENABLE_HTTPS=$ENABLE_HTTPS \
  -DCMAKE_CXX_FLAGS="-std=c++11 ${CMAKE_CXX_FLAGS}" \
  ..
make -j2
make test
cd ..
