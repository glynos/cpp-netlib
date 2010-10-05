.. _http_client:

*************
 HTTP client
*************

.. todo::

    Rewrite text, provide a CLI example.

The first code example is the simplest thing you can do with the
:mod:`cpp-netlib`.  The application is a simple HTTP client, which can
be found in the subdirectory ``libs/network/example/http_client.cpp``.
All we are doing is creating and sending an HTTP request to a server
and printing the response body.  Without further ado, the code to do
this is as follows:

.. code-block:: c++

    #include <boost/network/protocol/http/client.hpp>
    #include <iostream>
    
    int
    main(int argc, char *argv[]) {
        using namespace boost::network;
	
	if (argc != 2) {
	    std::cout << "Usage: " << argv[0] << " [url]" << std::endl;
    	    return 1;
        }
	
        http::client client;
        http::client::request request(argv[1]);
	request << header("Connection", "close");
	http::client::response response = client.get(request);
	std::cout << body(response) << std::endl;
	
  	return 0;
    }

Since this is the first example, each line will be presented and
explained in detail.

.. code-block:: c++

    #include <boost/network/protocol/http/client.hpp>

All the code needed for the HTTP client resides in this header.

.. code-block:: c++

    http::client client;

First we create a ``client`` object.  The ``client`` contains all the
connection and protocol logic.  The default HTTP client is version
1.1, as specified in `RFC 2616`_.

.. code-block:: c++

    http::client::request request(argv[1]);

Next, we create a ``request`` object, with a URI string passed as a
constructor argument.

.. code-block:: c++

    request << header("Connection", "close");

:mod:`cpp-netlib` makes use of stream syntax and *directives* to allow
developers to build complex message structures with greater
flexibility and clarity.  Here, we add the HTTP header "Connection:
close" to the request in order to signal that the connection will be
closed after the request has completed.

.. code-block:: c++

    http::client::response response = client.get(request);

Once we've built the request, we then make an HTTP GET request
throught the ``http::client`` from which an ``http::response`` is
returned.  ``http::client`` supports all common HTTP methods: GET,
POST, HEAD, DELETE.

.. code-block:: c++

    std::cout << body(response) << std::endl;

Finally, though we don't do any error checking, the response body is
printed to the console using the ``body`` directive.

That's all there is to the HTTP client.  In fact, it's possible to
compress this to a single line:

.. code-block:: c++

   std::cout << body(http::client().get(http::request("http://www.boost.org/")));

Next we'll develop a simple client/server application using
``http::server`` and ``http::client``.

.. _`RFC 2616`: http://www.w3.org/Protocols/rfc2616/rfc2616.html
