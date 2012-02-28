
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

As of 0.9.1 the default implementation for the :mod:`cpp-netlib` HTTP client is
asynchronous.

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

.. include:: ../in_depth/http_client_tags.rst

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

+----------------------+-------------------------------+-------------------------+
| Parameter Name       | Type                          | Description             |
+======================+===============================+=========================+
| _follow_redirects    | ``bool``                      | Boolean to specify      |
|                      |                               | whether the client      |
|                      |                               | should follow HTTP      |
|                      |                               | redirects. Default is   |
|                      |                               | ``false``.              |
+----------------------+-------------------------------+-------------------------+
| _cache_resolved      | ``bool``                      | Boolean to specify      |
|                      |                               | whether the client      |
|                      |                               | should cache resolved   |
|                      |                               | endpoints. The default  |
|                      |                               | is ``false``.           |
+----------------------+-------------------------------+-------------------------+
| _io_service          | ``boost::asio::io_service &`` | Reference to an         |
|                      |                               | instance of a           |
|                      |                               | Boost.Asio              |
|                      |                               | ``io_service``.         |
+----------------------+-------------------------------+-------------------------+
| _openssl_certificate | string                        | The filename of the     |
|                      |                               | certificate to load for |
|                      |                               | the SSL connection for  |
|                      |                               | verification.           |
+----------------------+-------------------------------+-------------------------+
| _openssl_verify_path | string                        | The directory from      |
|                      |                               | which the certificate   |
|                      |                               | authority files are     |
|                      |                               | located.                |
+----------------------+-------------------------------+-------------------------+


To use the above supported named parameters, you'll have code that looks like
the following:

.. code-block:: c++

    using namespace boost::network::http; // parameters are in this namespace
    boost::asio::io_service my_io_service;
    client client_(_follow_redirects=true, _cache_resolved=true,
                   _io_service=my_io_service
                   , _openssl_certificate="/tmp/my-cert"
                   , _openssl_verify_path="/tmp/ca-certs/");
    // use client_ as normal from here on out.

HTTP Methods
~~~~~~~~~~~~

The client implementation supports various HTTP methods. The following
constructs assume that a client has been properly constructed named ``client_``
and that there is an appropriately constructed request object named ``request_``
and that there is an appropriately constructed response object named
``response_`` like the following:

.. code-block:: c++

    using namespace boost::network::http;  // parameters are here
    client client_();
    client::request request_("http://cpp-netib.github.com/");
    client::response response_;

``response_ = client_.get(request_)``
    Perform an HTTP GET request.
``response_ = client_.get(request_, _body_handler=callback)``
    Perform an HTTP GET request, and have the body chunks be handled by the
    ``callback`` parameter. The signature of ``callback`` should be the following:
    ``void(iterator_range<char const *> const &, boost::system::error_code const
    &)``.
``response_ = client_.head(request_)``
    Perform an HTTP HEAD request.
``response_ = client_.post(request_)``
    Perform an HTTP POST, use the data already set in the request object which
    includes the headers, and the body.
``response_ = client_.post(request_, _body_handler=callback)``
    Perform an HTTP POST request, and have the body chunks be handled by the
    ``callback`` parameter. The signature of ``callback`` should be the following:
    ``void(iterator_range<char const *> const &, boost::system::error_code const
    &)``.
``response_ = client_.post(request_, body)``
    Body is a string of type ``boost::network::string<Tag>::type`` where ``Tag``
    is the HTTP Client's ``Tag``. The default content-type used is
    ``x-application/octet-stream``.
``response_ = client_.post(request_, body, _body_handler=callback)``
    Body is a string of type ``boost::network::string<Tag>::type`` where ``Tag``
    is the HTTP Client's ``Tag``. The default content-type used is
    ``x-application/octet-stream``. Have the response body chunks be handled by
    the ``callback`` parameter. The signature of ``callback`` should be the
    following: ``void(iterator_range<char const *> const &,
    boost::system::error_code const &)``.
``response_ = client_.post(request_, content_type, body)``
    The body and content_type parameters are of type
    ``boost::network::string<Tag>::type`` where ``Tag`` is the HTTP Client's
    ``Tag``. This uses the request object's other headers.
``response_ = client_.post(request_, content_type, body, _body_handler=callback)``
    The body and content_type parameters are of type
    ``boost::network::string<Tag>::type`` where ``Tag`` is the HTTP Client's
    ``Tag``. This uses the request object's other headers. Have the response
    body chunks be handled by the ``callback`` parameter. The signature of
    ``callback`` should be the following: ``void(iterator_range<char const *> const
    &, boost::system::error_code const &)``.
``response_ = client_.put(request_)``
    Perform an HTTP PUT, use the data already set in the request object which
    includes the headers, and the body.
``response_ = client_.put(request_, _body_handler=callback)``
    Perform an HTTP PUT request, and have the body chunks be handled by the
    ``callback`` parameter. The signature of ``callback`` should be the following:
    ``void(iterator_range<char const *> const &, boost::system::error_code const
    &)``.
``response_ = client_.put(request_, body)``
    Body is a string of type ``boost::network::string<Tag>::type`` where ``Tag``
    is the HTTP Client's ``Tag``. The default content-type used is
    ``x-application/octet-stream``.
``response_ = client_.put(request_, body, _body_handler=callback)``
    Body is a string of type ``boost::network::string<Tag>::type`` where ``Tag``
    is the HTTP Client's ``Tag``. The default content-type used is
    ``x-application/octet-stream``. Have the response body chunks be handled by
    the ``callback`` parameter. The signature of ``callback`` should be the
    following: ``void(iterator_range<char const *> const &,
    boost::system::error_code const &)``.
``response_ = client_.put(request_, content_type, body)``
    The body and content_type parameters are of type
    ``boost::network::string<Tag>::type`` where ``Tag`` is the HTTP Client's
    ``Tag``. This uses the request object's other headers.
``response_ = client_.put(request_, content_type, body, _body_handler=callback)``
    The body and content_type parameters are of type
    ``boost::network::string<Tag>::type`` where ``Tag`` is the HTTP Client's
    ``Tag``. This uses the request object's other headers. Have the response
    body chunks be handled by the ``callback`` parameter. The signature of
    ``callback`` should be the following: ``void(iterator_range<char const *> const
    &, boost::system::error_code const &)``.
``response_ = client_.delete_(request_)``
    Perform an HTTP DELETE request.
``response_ = client_.delete_(request_, _body_handler=callback)``
    Perform an HTTP DELETE request, and have the response body chunks be handled
    by the ``callback`` parameter. The signature of ``callback`` should be the
    following: ``void(iterator_range<char const *> const &,
    boost::system::error_code const &)``.

Client-Specific
~~~~~~~~~~~~~~~

``client_.clear_resolved_cache()``
    Clear the cache of resolved endpoints.


Streaming Body Handler
~~~~~~~~~~~~~~~~~~~~~~

As of v0.9.1 the library now offers a way to support a streaming body callback
function in all HTTP requests that expect a body part (GET, PUT, POST, DELETE).
A convenience macro is also provided to make callback handlers easier to write.
This macro is called ``BOOST_NETWORK_HTTP_BODY_CALLBACK`` which allows users to
write the following code to easily create functions or function objects that
are compatible with the callback function requirements.

An example of how to use the macro is shown below:

.. code-block:: c++

    struct body_handler {
        explicit body_handler(std::string & body)
        : body(body) {}

        BOOST_NETWORK_HTTP_BODY_CALLBACK(operator(), range, error) {
            // in here, range is the Boost.Range iterator_range, and error is
            // the Boost.System error code.
            if (!error)
                body.append(boost::begin(range), boost::end(range));
        }

        std::string & body;
    };

    // somewhere else
    std::string some_string;
    response_ = client_.get(request("http://cpp-netlib.github.com/"),
                            _body_handler=body_handler(some_string));

You can also use if for standalone functions instead if you don't want or need
to create a function object.

.. code-block:: c++

    BOOST_NETWORK_HTTP_BODY_CALLBACK(print_body, range, error) {
        if (!error)
            std::cout << "Received " << boost::distance(range) << "bytes."
                      << std::endl;
        else
            std::cout << "Error: " << error << std::endl;
    }

    // somewhere else
    response_ = client_.get(request("http://cpp-netlib.github.com/"),
                            _body_handler=print_body);

The ``BOOST_NETWORK_HTTP_BODY_CALLBACK`` macro is defined in
``boost/network/protocol/http/client/macros.hpp``.
