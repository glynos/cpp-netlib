.. _index:
.. rubric:: Straightforward network programming in modern C++

.. :Authors: Glyn Matthews <glyn.matthews@gmail.com>
.. 	  Dean Michael Berris <mikhailberis@gmail.com>
.. :Date: Feb 22, 2012
.. :Version: 0.9.3
.. :Description: Complete user documentation, with examples, for the :mod:`cpp-netlib`.
.. :Copyright: Copyright Glyn Matthews, Dean Michael Berris 2008-2012.
..             Distributed under the Boost Software License, Version
..             1.0. (See accompanying file LICENSE_1_0.txt or copy at
..             http://www.boost.org/LICENSE_1_0.txt)

Getting cpp-netlib
==================

**Download**

You can download the latest releases of the library at:

    http://github.com/cpp-netlib/cpp-netlib/downloads

You can find more information about the progress of the development by
checking our GitHub_ project page at:

    http://github.com/cpp-netlib/cpp-netlib

**Support**

You can ask questions, join the discussion, and report issues to the
developers mailing list by joining via:

    https://groups.google.com/group/cpp-netlib

You can also file issues on the Github_ issue tracker at:

    http://github.com/cpp-netlib/cpp-netlib/issues

We are a growing community and we are happy to accept new
contributions and ideas.

Boost
=====

The :mod:`cpp-netlib` is being developed for eventual submission to Boost_.

.. image:: _static/boost.png
   	   :align: left

C++ Network Library
===================

The :mod:`cpp-netlib` is a library that provides application layer
protocol support using modern C++ techniques.  It is light-weight,
fast, portable and is intended to be as easy to configure as possible.

It is developed by people linked to the Boost_ community and will soon
be submitted for review into Boost.  A presentation about
:mod:`cpp-netlib` was given at `BoostCon 2010`_, for which the
`slides`_ and the `paper`_ can be found on-line.

Hello, world!
=============

The :mod:`cpp-netlib` allows developers to write fast, portable
network applications with the minimum of fuss.

An HTTP server-client example can be written in tens of lines of code.
The client is as simple as this:

.. code-block:: c++

    using namespace boost::network;
    using namespace boost::network::http;

    client::request request_("http://127.0.0.1:8000/");
    request_ << header("Connection", "close");
    client client_;
    client::response response_ = client_.get(request_);
    std::string body_ = body(response_);

And the corresponding server code is listed below:

.. code-block:: c++

    namespace http = boost::network::http;

    struct handler;
    typedef http::server<handler> http_server;

    struct handler {
        void operator() (http_server::request const &request,
                         http_server::response &response) {
            response = http_server::response::stock_reply(
                http_server::response::ok, "Hello, world!");
        }

        void log(http_server::string_type const &info) {
            std::cerr << "ERROR: " << info << '\n';
        }
    };

    int main(int arg, char * argv[]) {
        handler handler_;
        http_server server_("0.0.0.0", "8000", handler_);
        server_.run();
    }

Want to learn more?
===================

    * :ref:`Take a look at the getting started guide <getting_started>`
    * :ref:`Learn from some simple examples <examples>`
    * :ref:`Find out what's new <whats_new>`
    * :ref:`Study the library in more depth <in_depth>`
    * :ref:`Discover more through the full reference <reference>`

.. warning:: Be aware that not all features are stable.  The generic
   	     message design is under review and the URI and HTTP
   	     client implementation will continue to undergo
   	     refactoring.  Future versions will include support for
   	     other network protocols.


.. _Boost: http://www.boost.org/
.. _`BoostCon 2010`: http://www.boostcon.com/
.. _`slides`: http://www.filetolink.com/b0e89d06
.. _`paper`: http://github.com/downloads/mikhailberis/cpp-netlib-boostcon-paper/cpp-netlib.pdf
.. _Git: http://git-scm.com/
.. _GitHub: http://github.com/

