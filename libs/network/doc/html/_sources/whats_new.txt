.. _whats_new:

************
 What's New
************

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
