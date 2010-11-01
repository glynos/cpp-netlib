.. cpp-netlib documentation master file, created by
   sphinx-quickstart on Wed Jun 16 23:53:37 2010.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

=====================
 C++ Network Library
=====================

.. image:: static/boost.png
   :align: right

.. :Authors: Glyn Matthews <glyn.matthews@gmail.com>
.. 	  Dean Michael Berris <mikhailberis@gmail.com>
.. :Date: Nov 1, 2010
.. :Version: 0.8
.. :Description: Complete user documentation, with examples, for the :mod:`cpp-netlib`.
.. :Copyright: Copyright Glyn Matthews, Dean Michael Berris 2008-2010.
..             Distributed under the Boost Software License, Version
..             1.0. (See accompanying file LICENSE_1_0.txt or copy at
..             http://www.boost.org/LICENSE_1_0.txt)

Welcome
-------

The :mod:`cpp-netlib` is a library that provides application layer
protocol support using modern C++ techniques.  It is light-weight,
fast, cross-platform and is intended to be as easy to configure as
possible.

It is developed by people linked to the Boost_ community and will at
some point in the future be submitted for review into Boost.  A
presentation about :mod:`cpp-netlib` was given at `BoostCon 2010`_,
for which the `slides`_ and the `paper`_ can be found on-line.

Sneak Peak
----------

The :mod:`cpp-netlib` allows you to write semantically consistent code for
making different kinds of higher level network applications.

The library allows for writing simple code for simple C++ HTTP client
applications like:

.. code-block:: c++

    using namespace boost::network;
    using namespace boost::network::http;

    client::request request_("http://www.boost.org/");
    request_ << header("Connection", "close");
    client client_;
    client::response response_ = client_.get(request);
    std::string body = body(response_);

The :mod:`cpp-netlib` is being developed for eventual submission to Boost_.

Download
--------

You can download the latest releases of the library at:

    http://github.com/cpp-netlib/cpp-netlib/downloads

You can also get the latest developments from the Git_ repository at:

    git://github.com/mikhailberis/cpp-netlib.git

You can find more information about the progress of the development by checking
the GitHub_ project page at:

    http://github.com/cpp-netlib/cpp-netlib

Support
-------

You can ask questions, join the discussion, and report issues to the developers
mailing list by joining via:

    https://lists.sourceforge.net/lists/listinfo/cpp-netlib-devel

You may also file issues on the Github_ issue tracker at:

    http://github.com/cpp-netlib/cpp-netlib/issues

Contents
--------

.. toctree::
   :maxdepth: 2

   whats_new.rst
   getting_started.rst
   examples.rst
   in_depth.rst
   techniques.rst
   history.rst
   install.rst
   reference.rst
   references.rst

.. _Boost: http://www.boost.org/
.. _`BoostCon 2010`: http://www.boostcon.com/
.. _`slides`: http://www.filetolink.com/b0e89d06
.. _`paper`: http://github.com/downloads/mikhailberis/cpp-netlib-boostcon-paper/cpp-netlib.pdf
.. _Git: http://git-scm.com/
.. _GitHub: http://github.com/

