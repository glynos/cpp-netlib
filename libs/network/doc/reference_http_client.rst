
HTTP Client API
===============

General
-------

:mod:`cpp-netlib` includes and implements a number of HTTP clients that you can
use and embed in your own applications. All of the HTTP client implementations:

* Cannot be copied. This means you may have to store instances of the clients in
  dynamic memory.
* Assume that requests made are independent of each other. There currently is no
  cookie or session management system built-in to cpp-netlib's HTTP client
  implementations.
* Are header-only and are compiled-into your application. Future releases in
  case you want to upgrade the implementation you are using in your application
  will be distributed as header-only implementations, which means you have to
  re-compile your application to use a newer version of the implementations.

The HTTP clients all share the same API, but the internals are documented in
terms of what is different and what to expect with the different
implementations.

Implementations
---------------

.. FIXME describe the internal implementation, with diagrams if at all possible
   and within reasonable efforts.

Synchronous Clients
~~~~~~~~~~~~~~~~~~~

.. FIXME show the table of tags that enable the synchronous implementation.

Asynchronous Clients
~~~~~~~~~~~~~~~~~~~~

.. FIXME show the table of tags that enable the asynchronous implementation.

Member Functions
----------------

.. FIXME show the table of publicly-accessible member functions.


