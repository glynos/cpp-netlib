Objectives
==========


The objectives of the :mod:`cpp-netlib` are to:

* develop a high quality, portable, easy to use C++ networking library
* enable developers to easily extend the library
* lower the barrier to entry for cross-platform network-aware C++
  applications

The goal the of :mod:`cpp-netlib` has never been to build a
fully-featured web server - there are plenty of excellent options
already available.  The niche that this library targets is for
light-weight networking functionality for C++ applications that have
demanding performance requirements or memory constraints, but that
also need to be portable.  This type of application is becoming
increasingly common as software becomes more distributed, and
applications need to communicate with services.

While many languages provide direct library support for high level
network programming, this feature is missing in C++.  Therefore, this
library has been developed with the intention of eventually being
submitted to Boost_, a collection of general, high quality
libraries for C++ developers.

.. _Boost: http://www.boost.org/

Eventually, the :mod:`cpp-netlib` will be extended to support many of
the application layer protocols such as SMTP, FTP, SOAP, XMPP etc.
