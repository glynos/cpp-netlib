
HTTP Server API
===============

General
-------

:mod:`cpp-netlib` includes and implements two distinct HTTP server
implementations that you can use and embed in your own applications. Both HTTP
Server implementations:

  * **Cannot be copied.** This means you may have to store instances of the HTTP
    Server in dynamic memory if you intend to use them as function parameters or
    pass them around in smart pointers of by reference.
  * **Assume that requests made are independent of each other.** None of the
    HTTP Server implementations support request pipelining (yet) so a single
    connection only deals with a single request.
  * **Are header-only and are compiled-into your application.** Future releases
    in case you want to upgrade the implementation you are using in your
    application will be distributed as header-only implementations, which means
    you have to re-compile your application to use a newer version of the
    implementations.

The HTTP Servers have different semantics, and in some cases require different
APIs from the supplied template parameters.

Implementations
---------------

There are two different user-facing template classes that differentiate the
`Synchronous Servers`_ from the `Asynchronous Servers`_. Both templates take a
single template parameter named ``Handler`` which describes the type of the
Handler function object.

There are two different Handler concepts, one concept for `Synchronous Servers`_
and another for `Asynchronous Servers`.

The SynchronusHandler concept for `Synchronous Servers`_ is described by the
following table:

---------------

**Legend:**

H
    The Handler type.
h
    An instance of H.
Req
    A type that models the Request Concept.
Res
    A type that models the Response Concept.
req
    An instance of Req.
res
    An instance of Res.

+----------------+-------------+----------------------------------------------+
| Construct      | Return Type | Description                                  |
+================+=============+==============================================+
| ``h(req,res)`` | ``void``    | Handle the request; res is passed in as a    |
|                |             | non-const lvalue, which represents the       |
|                |             | response to be returned to the client        |
|                |             | performing the request.                      |
+----------------+-------------+----------------------------------------------+

More information about the internals of the `Synchronous Servers`_ can be found
in the following section.

The AsynchronousHandler concept for `Asynchronous Servers`_ is described by the
following table:

---------------

**Legend:**

H
    The Handler type.
h
    An instance of H.
Req
    A type that models the Request Concept.
ConnectionPtr
    A type that models the Connection Pointer Concept.
req
    An instance of Req.
conn
    An instance of ConncetionPtr.

+------------------+-------------+--------------------------------------------+
| Construct        | Return Type | Description                                |
+==================+=============+============================================+
| ``h(req, conn)`` | ``void``    | Handle the request; conn is a shared       |
|                  |             | pointer which exposes functions for        |
|                  |             | writing to and reading from the connection.|
+------------------+-------------+--------------------------------------------+

More information about the internals of the `Asynchronous Servers`_ can be found
in the following section.

Synchronous Servers
~~~~~~~~~~~~~~~~~~~

.. FIXME show the table of tags that enable the synchronous implementation.

Asynchronous Servers
~~~~~~~~~~~~~~~~~~~~

.. FIXME show the table of tags that enable the asynchronous implementation.

Member Functions
----------------

.. FIXME show the table of publicly-accessible member functions.


