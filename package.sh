#!/bin/sh
#
# Copyright 2012 Dean Michael Berris <dberris@google.com>
# Copyright 2012 Google, Inc.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)
#

VERSION="$1"
if [ "$VERSION" == "" ]; then
  VERSION="`git log --format=oneline | awk '{print $1}' | head -1`"
fi

TAG="cpp-netlib-$VERSION"
git tag $TAG
echo "Tagged $TAG."

git archive --prefix=cpp-netlib-$VERSION/ --format=zip $TAG >cpp-netlib-$VERSION.zip
git archive --prefix=cpp-netlib-$VERSION/ --format=tar $TAG | gzip >cpp-netlib-$VERSION.tar.gz
git archive --prefix=cpp-netlib-$VERSION/ --format=tar $TAG | bzip2 >cpp-netlib-$VERSION.tar.bz2
echo "Packaged $TAG."
