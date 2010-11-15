
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

The synchronous server implementation is represented by the template ``server``
in namespace ``boost::network::http``. The ``server`` template takes in a single
template parameter named ``Handler`` which models the SynchronousHandler
concept (described above).

An instance of Handler is taken in by reference to the constructor of the HTTP
server. This means the Handler is not copied around and only a single instance
of the handler is used for all connections and requests performed against the
HTTP server.

.. warning:: It is important to note that the HTTP server does not implement any
   locking upon invoking the Handler. In case you have any state in the Handler
   that will be associated with the synchronous server, you would have to
   implement your own synchronization internal to the Handler implementation.
   This matters especially if you run the synchronous server in multiple
   threads.

The general pattern of usage for the HTTP Server template is shown below:

.. code-block:: c++

    struct handler;
    typedef boost::network::http::server<handler> http_server;

    struct handler {
        void operator()(
            http_server::request const & req,
            http_server::response & res
        ) {
            // do something, and then edit the res object here.
        }
    };

More information about the actual HTTP Server API follows in the next section.
It is important to understand that the HTTP Server is actually embedded in your
application, which means you can expose almost all your application logic
through the Handler type, which you can also initialize appropriately.

API Documentation
`````````````````

The following sections assume that the following file has been included:

.. code-block:: c++
    
    #include <boost/network/include/http/server.hpp>

And that the following typedef's have been put in place:

.. code-block:: c++

    struct handler_type;
    typedef boost::network::http::server<handler_type> http_server;

Constructor
***********

``http_server(address, port, handler)``
    Construct an HTTP Server instance, passing in the address and port as
    ``std::string const &`` and handler being of type ``handler_type`` but
    passed in as an lvalue reference.

Public Members
**************

The following definitions assume that a properly constructed ``http_server``
instance has been constructed in the following manner:

.. code-block:: c++

    handler_type handler;
    http_server server("127.0.0.1", "8000", handler);

``server.run()``
    Run the HTTP Server event loop. This function can be run on multiple threads
    following the example:

.. code-block:: c++

    boost::thread t1(boost::bind(&http_server::run, &server));
    boost::thread t2(boost::bind(&http_server::run, &server));
    server.run();

``server.stop()``
    Stop the HTTP Server acceptor and wait for all pending requests to finish.

Asynchronous Servers
~~~~~~~~~~~~~~~~~~~~

.. FIXME show the table of tags that enable the asynchronous implementation.

API Documentation
`````````````````

.. FIXME show the table of publicly-accessible member functions.

