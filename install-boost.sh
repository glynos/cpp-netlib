#!/bin/sh
set -e

if [ ! -d "${HOME}/${CC}-boost_${BOOST_VERSION}/include" ]; then
  wget -O boost_${BOOST_VERSION}.tar.bz2 http://sourceforge.net/projects/boost/files/boost/${BOOST_VER}/boost_${BOOST_VERSION}.tar.bz2/download
  tar jxf boost_${BOOST_VERSION}.tar.bz2
  cd boost_${BOOST_VERSION}
  ./bootstrap.sh --with-toolset=$TOOLSET --prefix=${HOME}/${CC}-boost_${BOOST_VERSION}
  ./b2 --stagedir=. -j2 --build-type=complete --layout=tagged cxxflags='-std=c++11' install 2>boost-error.log
  cd ..
  rm -rf boost_${BOOST_VERSION}
  rm -rf boost_${BOOST_VERSION}.tar.bz2
fi
