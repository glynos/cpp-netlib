.. _getting_started:

*****************
 Getting started
*****************

Installing an official release
==============================

All stable versions of :mod:`cpp-netlib` can be downloaded from Github:
``http://github.com/cpp-netlib/cpp-netlib/downloads``.  Each release
is available as gzipped (Using the command ``tar xzf
cpp-netlib.tar.gz``) or bzipped (Using ``tar xjf cpp-netlib.tar.bz2``)
tarball, or as a zipfile (``unzip cpp-netlib.zip``, or on Windows
using a tool such as 7zip_).

.. _7zip: http://www.7-zip.org/

Installing a development version
================================

The :mod:`cpp-netlib` uses Git_ for source control, so to use any
development versions Git must be installed on your system.

Using the command line, the command to get the latest code is:

::
    shell$ git clone git://github.com/mikhailberis/cpp-netlib.git

This should be enough information get to started.  To do more complex
things with Git, such as pulling changes or checking out a new branch,
refer to the `Git documentation`_.

Windows users need to use msysGit_, and to invoke the command above
from a shell.

For fans of Subversion_, the same code can be checked out from
`http://svn.github.com/mikhailberis/cpp-netlib.git`.

.. _Git: http://git-scm.com/
.. _`Git documentation`: http://git-scm.com/documentation
.. _msysGit: http://code.google.com/p/msysgit/downloads/list
.. _Subversion: http://subversion.tigris.org/

Getting Boost
=============

:mod:`cpp-netlib` depends on Boost_.  It should work for Boost 1.41.0 and
above.  If Boost is not installed on your system, the environment
variable ``BOOST_ROOT`` must be defined, which must be the full path
name of the top directory of the Boost distribution. Although Boost is
mostly header only, applications built using :mod:`cpp-netlib` still
requires linking with `Boost.System`_.

.. _Boost: http://www.boost.org/doc/libs/release/more/getting_started/index.html
.. _`Boost.System`: http://www.boost.org/libs/system/index.html

Getting CMake
=============

The :mod:`cpp-netlib` uses CMake_.

.. _CMake: http://www.cmake.org/

Instructions for Windows Users
==============================

Instructions for Linux Users
============================

Building and Running the Examples
=================================

