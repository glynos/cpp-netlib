
HTTP Server API
===============

General
-------

:mod:`cpp-netlib` includes and implements and asynchronous HTTP server
implementation that you can use and embed in your own applications. The HTTP
Server implementation:

  * **Cannot be copied.** This means you may have to store instances of the HTTP
    Server in dynamic memory if you intend to use them as function parameters or
    pass them around in smart pointers of by reference.
  * **Assume that requests made are independent of each other.** None of the
    HTTP Server implementations support request pipelining (yet) so a single
    connection only deals with a single request.
  * **The Handler instance is invoked asynchronously**. This means the I/O
    thread used to handle network-related events are free to handle only the
    I/O related events. This enables the server to scale better as to the
    number of concurrent connections it can handle.
  * **The Handler is able to schedule asynchronous actions on the thread pool
    associated with the server.** This allows handlers to perform multiple
    asynchronous computations that later on perform writes to the connection.
  * **The Handler is able to control the (asynchronous) writes to and reads
    from the HTTP connection.** Because the connection is available to the
    Handler, that means it can write out chunks of data at a time or stream
    data through the connection continuously.

The Handler concept for the HTTP Server is described by the following table:

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

---------------

The HTTP Server is meant to allow for better scalability in terms of the number
of concurrent connections and for performing asynchronous actions within the
handlers.  The HTTP Server implementation is available from a single
user-facing template named ``server``. This template takes in a single template
parameter which is the type of the Handler to be called once a request has been
parsed from a connection.

An instance of Handler is taken as a reference to the constructor of the server
instance.

.. warning:: The HTTP Server implementation does not perform any
   synchronization on the calls to the Handler invocation. This means if your
   handler contains or maintains internal state, you are responsible for
   implementing your own synchronization on accesses to the internal state of
   the Handler.

The general pattern for using the ``server`` template is shown below:

.. code-block:: c++

    struct handler;
    typedef boost::network::http::server<handler> http_server;

    struct handler {
        void operator()(
            http_server::request const & req,
            http_server::connection_ptr connection
        ) {
            // handle the request here, and use the connection to
            // either read more data or write data out to the client
        }
    };

API Documentation
-----------------

The following sections assume that the following file has been included:

.. code-block:: c++

    #include <boost/network/include/http/server.hpp>
    #include <boost/network/utils/thread_pool.hpp>

And that the following typedef's have been put in place:

.. code-block:: c++

    struct handler_type;
    typedef boost::network::http::server<handler_type> http_server;

    struct handler_type {
        void operator()(http_server::request const & request,
                        http_server::connection_ptr connection) {
            // do something here
        }
    };

Constructor
~~~~~~~~~~~

``explicit http_server(options)``
    Construct an HTTP server instance passing in a ``server_options<Tag,
    Handler>`` instance.

Server Options
~~~~~~~~~~~~~~

.. doxygenstruct:: boost::network::http::server_options
   :project: cppnetlib
   :members:

Public Members
~~~~~~~~~~~~~~

.. doxygenstruct:: boost::network::http::server
   :project: cppnetlib
   :members:
   :undoc-members:

Connection Object
~~~~~~~~~~~~~~~~~

.. doxygenstruct:: boost::network::http::async_connection
   :project: cppnetlib
   :members:

Adding SSL support to the HTTP Server
-------------------------------------

In order to setup SSL support for an Asynchronous Server, it is best to start from
a regular Asynchronous Server (see above). Once this server is setup, SSL can be
enabled by adding a Boost.Asio.Ssl.Context_ to the options. The settings that can be
used are defined in the link.

.. code-block:: c++

    // Initialize SSL context
    std::shared_ptr<asio::ssl::context> ctx =
        std::make_shared<asio::ssl::context>(asio::ssl::context::sslv23);
    ctx->set_options(
                asio::ssl::context::default_workarounds
                | asio::ssl::context::no_sslv3
                | asio::ssl::context::single_dh_use);
    
    // Set keys
    ctx->set_password_callback(password_callback);
    ctx->use_certificate_chain_file("server.pem");
    ctx->use_private_key_file("server.pem", asio::ssl::context::pem);
    ctx->use_tmp_dh_file("dh512.pem");
    
    handler_type handler;
    http_server::options options(handler);
    options.thread_pool(std::make_shared<boost::network::utils::thread_pool>(2));
    http_server server(options.address("127.0.0.1").port("8442").context(ctx));

    std::string password_callback(std::size_t max_length, asio::ssl::context_base::password_purpose purpose) {
        return std::string("test");
    }
    
.. _Boost.Range: http://www.boost.org/libs/range
.. _Boost.Function: http://www.boost.org/libs/function
.. _Boost.Asio.SSL.Context: http://www.boost.org/doc/libs/1_55_0/doc/html/boost_asio/reference/ssl__context.html
