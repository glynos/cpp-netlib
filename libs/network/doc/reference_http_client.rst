
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

.. FIXME show the table of publicly-accessible member functions.


