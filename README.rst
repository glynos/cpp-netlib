C++ Network Library
===================

Modern C++ network programming libraries.

.. image:: https://travis-ci.org/cpp-netlib/cpp-netlib.svg?branch=master
    :target: https://travis-ci.org/cpp-netlib/cpp-netlib

.. image:: https://scan.coverity.com/projects/6714/badge.svg
    :target: https://scan.coverity.com/projects/cpp-netlib

Join us on Slack: http://slack.cpp-netlib.org/

Subscribe to the mailing list: https://groups.google.com/forum/#!forum/cpp-netlib

Introduction
------------

cpp-netlib is a collection of network related routines/implementations
geared towards providing a robust cross-platform networking library.
cpp-netlib offers the following implementations:

  *  Common Message Type -- A generic message type which can be used
     to encapsulate and store message related information, used by all
     network implementations as the primary means of data exchange.
  *  Network protocol message parsers -- A collection of parsers which
     generate message objects from strings.
  *  Adapters and Wrappers -- A collection of Adapters and wrappers aimed
     towards making the message type STL friendly.
  *  Network protocol client and server implementations -- A collection
     of network protocol implementations that include embeddable client
     and server types.

This library is released under the Boost Software License (please see
http://boost.org/LICENSE_1_0.txt or the accompanying LICENSE_1_0.txt file
for the full text.

Downloading cpp-netlib
----------------------

You can find official release packages of the library at::

    http://github.com/cpp-netlib/cpp-netlib/downloads

Building and Installing
-----------------------

Building with CMake
~~~~~~~~~~~~~~~~~~~

To build the libraries and run the tests with CMake, you will need to
have CMake version 2.8 or higher installed appropriately in your
system.

::

    $ cmake --version
    cmake version 2.8.1

Inside the cpp-netlib directory, you can issue the following statements to
configure and generate the Makefiles, and build the tests::

    $ cd ~/cpp-netlib      # we're assuming it's where cpp-netlib is
    $ cmake -DCMAKE_BUILD_TYPE=Debug     \
    >       -DCMAKE_C_COMPILER=clang     \
    >       -DCMAKE_CXX_COMPILER=clang++ \
    >    .

Once CMake is done with generating the Makefiles and configuring the project,
you can now build the tests and run them::

    $ cd ~/cpp-netlib
    $ make
    $ make test

If for some reason some of the tests fail, you can send the files in
``Testing/Temporary/`` as attachments to the cpp-netlib `developers mailing
list`_.

.. _`developers mailing list`: cpp-netlib@googlegroups.com

Running Tests
-------------

If you want to run the tests that come with cpp-netlib, there are a few things
you will need. These are:

  * A compiler (GCC 4.x, Clang 2.8, MSVC 2008)
  * A build tool (CMake [#]_ recommended, Boost.Build also an option)
  * OpenSSL headers (optional)

.. note:: This assumes that you have cpp-netlib at the top-level of
          your home directory.
  [#] http://www.cmake.org/

Hacking on cpp-netlib
---------------------

cpp-netlib is being developed with the git_ distributed SCM system.
cpp-netlib is hosted on GitHub_ following the GitHub recommended practice of
forking the repository and submitting pull requests to the source repository.
You can read more about the forking_ process and submitting `pull requests`_ if
you're not familiar with either process yet.

.. _git: http://git-scm.com/
.. _GitHub: http://github.com/
.. _forking: http://help.github.com/forking/
.. _`pull requests`: http://help.github.com/pull-requests/

Because cpp-netlib is released under the `Boost Software License`_ it is
recommended that any file you make changes to bear your copyright notice
alongside the original authors' copyright notices on the file. Typically the
copyright notices are at the top of each file in the project.

.. _`Boost Software License`: http://www.boost.org/LICENSE_1_0.txt

At the time of writing, there are no coding conventions being followed but if
you write in the general style that is already existing in the project that
would be greatly appreciated. Copious amounts of comments will be called out,
but code that is not self-explanatory typically at least requires a rationale
documentation in comments explaining "why" the code is written that way.

The main "upstream" repository is the one hosted by the original maintainer of
the project (Dean Michael Berris) at http://github.com/deanberris/cpp-netlib.
The "official" release repository is maintained at
http://github.com/cpp-netlib/cpp-netlib -- which is a fork of the upstream
repository. It is recommended that forks be made against the upstream repostory
and pull requests be submitted against the upstream repository so that patches
and other implementations can be curated by the original maintainer.

Contact and Support
-------------------

In case you have any questions or would like to make feature requests, you can
contact the development team through the `developers mailing list`_
or by filing issues at http://github.com/cpp-netlib/cpp-netlib/issues.

Join us on Slack: http://slack.cpp-netlib.org/

.. _`developers mailing list`: cpp-netlib@googlegroups.com

You can reach the maintainers of the project through::

    Dean Michael Berris (dberris@google.com)

    Glyn Matthews (glyn.matthews@gmail.com)
