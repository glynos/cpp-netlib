.. _whats_new:

************
 What's New
************

:mod:`cpp-netlib` 0.9
---------------------

* **IMPORTANT BREAKING CHANGE**: By default all compile-time heavy parser 
  implementations are now compiled to external static libraries. In order to use
  :mod:`cpp-netlib` in header-only mode, users must define the preprocessor
  macro ``BOOST_NETWORK_NO_LIB`` before including any :mod:`cpp-netlib` header.
  This breaks code that relied on the version 0.8.x line where the library is
  strictly header-only.
* Fix issue #41: Introduce a macro ``BOOST_NETWORK_HTTP_CLIENT_DEFAULT_TAG`` 
  which makes the default HTTP client use ``tags::http_async_8bit_udp_resolve``
  as the tag.
* Fix issue #40: Write the status line and headers in a single buffer write
  instead of two writes.
* More consistent message API for client and server messages (request and
  response objects).
* Refactoring of internal implementations to allow better separation of concerns
  and more manageable coding/documentation.
* Client and Server constructors that support Boost.Parameter named parameters.
* Client and Server cosntructors now take in an optional reference to a Boost.Asio 
  ``io_service`` to use internally.
* Documentation updates to reflect new APIs.

:mod:`cpp-netlib` 0.8
---------------------

* Updates to URI unit tests and documentation.
* More documentation, covering the HTTP Client and HTTP Server APIs
* Asynchronous HTTP Server that now supports running request handlers on a
  different thread pool.
* An initial thread pool implementation, using Boost.Asio underneath.
* Adding a ready(...) wrapper to check whether a response object returned by 
  the asynchronous client in 0.7 already has all the parts available.
* Some attempts at lowering compile time costs.

:mod:`cpp-netlib` 0.7
---------------------

* Radical documentation overhaul
* Asynchronous HTTP client
* Tag dispatch overhaul, using Boost.MPL
* HTTP Client Facade refactoring
* Bug fixes for HTTP 1.1 response parsing
* Minimized code repetition with some header macro's
* Configurable HTTPS support in the library with ``BOOST_NETWORK_ENABLE_HTTPS``


:mod:`cpp-netlib` 0.6
---------------------

* Many fixes for MSVC compiler

:mod:`cpp-netlib` 0.5
---------------------

* An embeddable HTTP 1.1 server
* An HTTP 1.1 client upgraded to support HTTPS
* An updated URI parser implementation
* An asynchronous HTTP 1.1 client
* An HTTP 1.1 client that supports streaming function handlers
