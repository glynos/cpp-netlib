
HTTP Client API
===============

General
-------

:mod:`cpp-netlib` includes and implements a number of HTTP clients that you can
use and embed in your own applications. All of the HTTP client implementations:

  * **Cannot be copied.** This means you may have to store instances of the 
    clients in dynamic memory if you intend to use them as function parameters
    or pass them around in smart pointers or by reference.
  * **Assume that requests made are independent of each other.** There currently
    is no cookie or session management system built-in to cpp-netlib's HTTP client
    implementations.
  * **Are header-only and are compiled-into your application.** Future releases in
    case you want to upgrade the implementation you are using in your application
    will be distributed as header-only implementations, which means you have to
    re-compile your application to use a newer version of the implementations.
  
The HTTP clients all share the same API, but the internals are documented in
terms of what is different and what to expect with the different
implementations.

Implementations
---------------

There is a single user-facing template class named ``basic_client`` which takes
three template parameters:

  * **Tag** - which static tag you choose that defines the behavior of the client.

  * **http_version_major** - an unsigned int that defines the HTTP major version
    number, this directly affects the HTTP messages sent by the client.

  * **http_version_minor** - an unsigned int that defines the HTTP minor version
    number.

There are two major different class of implementations of the ``basic_client``
template that depend on which tag you choose: `Synchronous Clients`_ and
`Asynchronous Clients`_. These two different classes are described in their own
sections following this one. What follows is a table of all tags supported by
the HTTP client implementation provided by :mod:`cpp-netlib`.

---------------

.. include:: http_client_tags.rst

In the above table the tags follow a pattern for describing the behavior
introduced by the tags. This pattern is shown below:

    <protocol>_<modifier>_<character-width>_<resolve-strategy>

For example, the tag ``http_default_8bit_tcp_resolve`` indicates the protocol
``http``, a modifier ``default``, a character width of ``8bit``, and a resolve
strategy of ``tcp_resolve``.

Synchronous Clients
~~~~~~~~~~~~~~~~~~~

Of the client tags shown in the table, the following makes the ``basic_client``
behave as a fully synchronous client. 

  * **http_default_8bit_tcp_resolve**
  * **http_default_8bit_udp_resolve**
  * **http_keepalive_8bit_tcp_resolve**
  * **http_keepalive_8bit_udp_resolve**

The synchronous client implements all the operations of the client underneath 
the interface all block to wait for I/O to finish. All the member methods are
synchronous and will block until the response object is ready or throws if erros
are encountered in the performance of the HTTP requests.

.. warning:: The synchronous clients are **NOT** thread safe. You will need to do
   external synchronization to use synchronous client implementations.

Asynchronous Clients
~~~~~~~~~~~~~~~~~~~~

The following tags specify the ``basic_client`` to behave in an asynchronous
manner:

  * **http_async_8bit_tcp_resolve**
  * **http_async_8bit_udp_resolve**

An asynchronous client implementation means that``basic_client<...>`` is an 
`Active Object`_. This means that the client has and manages its own lifetime 
thread, and returns values that are asynchronously filled in. The response 
object encapsulates Boost.Thread_ futures which get filled in once the values 
are available.

.. _Boost.Thread: http://www.boost.org/libs/thread
.. _`Active Object`: http://en.wikipedia.org/wiki/Active_object

The asynchronous clients implement all operations asynchronously which are hidden
from the user. The interface is still synchronous but the fetching of data
happens on a different thread.

.. note:: The asynchronous clients are thread safe, and can be shared across
   many threads. Each request starts a sequence of asynchronous operations
   dedicated to that request. The client does not re-cycle connections and uses
   a one-request-one-connection model.

Member Functions
----------------

In this section we assume that the following typedef is in effect:

.. code-block:: c++

    typedef boost::network::http::basic_client<
        boost::network::http::tags::http_default_8bit_udp_resolve
        , 1
        ,1
        >
        client;

Also, that code using the HTTP client will have use the following header:

.. code-block:: c++

    #include <boost/network/include/http/client.hpp>

.. note:: Starting version 0.9, cpp-netlib clients and server implementations
   by default now have an externally-linked component. This is a breaking change
   for code that used to rely on cpp-netlib being a header-only library, but can
   inhibited by defining the ``BOOST_NETWORK_NO_LIB`` preprocessor macro before
   including any cpp-netlib header.

Constructors
~~~~~~~~~~~~

The client implementation can be default constructed, or customized at
initialization.

``client()``
    Default constructor.
``client(boost::asio::io_service & io_service)``
    Construct a client to use an existing Boost.Asio ``io_service`` instance.
``template <class ArgPack> client(ArgPack const & args)``
    Pass in an argument pack. See supported parameters in the table below.

+-------------------+-------------------------------+-------------------------+
| Parameter Name    | Type                          | Description             |
+===================+===============================+=========================+
| _follow_redirects | ``bool``                      | Boolean to specify      |
|                   |                               | whether the client      |
|                   |                               | should follow HTTP      |
|                   |                               | redirects. Default is   |
|                   |                               | ``false``.              |
+-------------------+-------------------------------+-------------------------+
| _cache_resolved   | ``bool``                      | Boolean to specify      |
|                   |                               | whether the client      |
|                   |                               | should cache resolved   |
|                   |                               | endpoints. The default  |
|                   |                               | is ``false``.           |
+-------------------+-------------------------------+-------------------------+
| _io_service       | ``boost::asio::io_service &`` | Reference to an         |
|                   |                               | instance of a           |
|                   |                               | Boost.Asio              |
|                   |                               | ``io_service``.         |
+-------------------+-------------------------------+-------------------------+

To use the above supported named parameters, you'll have code that looks like
the following:

.. code-block:: c++
    
    using namespace boost::network::http; // parameters are in this namespace
    boost::asio::io_service my_io_service;
    client client_(_follow_redirects=true, _cache_resolved=true,
                   _io_service=my_io_service);
    // use client_ as normal from here on out.

HTTP Methods
~~~~~~~~~~~~

The client implementation supports various HTTP methods. The following
constructs assume that a client has been properly constructed named ``client_``
and that there is an appropriately constructed request object named ``request_``
and that there is an appropriately constructed response object named
``response_`` like the following:

.. code-block:: c++

    client client_();
    client::request request_("http://cpp-netib.github.com/");
    client::response response_;

``response_ = client_.get(request_)``
    Perform an HTTP GET request.
``response_ = client_.head(request_)``
    Perform an HTTP HEAD request.
``response_ = client_.post(request_)``
    Perform an HTTP POST, use the data already set in the request object which
    includes the headers, and the body.
``response_ = client_.post(request_, body)``
    Body is a string of type ``boost::network::string<Tag>::type`` where ``Tag``
    is the HTTP Client's ``Tag``. The default content-type used is
    ``x-application/octet-stream``.
``response_ = client_.post(request_, content_type, body)``
    The body and content_type parameters are of type
    ``boost::network::string<Tag>::type`` where ``Tag`` is the HTTP Client's
    ``Tag``. This uses the request object's other headers.
``response_ = client_.put(request_)``
    Perform an HTTP PUT, use the data already set in the request object which
    includes the headers, and the body.
``response_ = client_.put(request_, body)``
    Body is a string of type ``boost::network::string<Tag>::type`` where ``Tag``
    is the HTTP Client's ``Tag``. The default content-type used is
    ``x-application/octet-stream``.
``response_ = client_.put(request_, content_type, body)``
    The body and content_type parameters are of type
    ``boost::network::string<Tag>::type`` where ``Tag`` is the HTTP Client's
    ``Tag``. This uses the request object's other headers.
``response_ = client_.delete_(request_)``
    Perform an HTTP DELETE request.

Client-Specific
~~~~~~~~~~~~~~~

``client_.clear_resolved_cache()``
    Clear the cache of resolved endpoints.


