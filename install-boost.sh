#!/bin/sh
set -e

if [ ! -d "${BOOST_ROOT}" ]; then
  wget -O boost_${BOOST_VERSION}.tar.bz2 http://sourceforge.net/projects/boost/files/boost/${BOOST_VER}/boost_${BOOST_VERSION}.tar.bz2/download
  tar jxf boost_${BOOST_VERSION}.tar.bz2
fi
