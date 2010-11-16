
HTTP Request
============

This part of the documentation talks about the publicly accessible API of the
HTTP Request objects. This section details the `Request Concepts`_ requirements,
the implemented and required Directives_, Modifiers_, and Wrappers_ that work
with the HTTP Request objects.

Request Concepts
----------------

There are two generally supported Request Concepts implemented in the library.
The first of two is the `Normal Client Request Concept`_ and the second is the
`Pod Server Request Concept`_.

The `Normal Client Request Concept`_ is what the HTTP Client interface requires.
All operations performed internally by the HTTP Client abide by the interface
required by this concept definition.

The `Pod Server Request Concept`_ is as the name suggests what the HTTP Server
implementation requires from Request Objects.

Switching on whether the `Request` concept chooses either of the `Normal Client
Request Concept`_ or the `Pod Server Request Concept`_ is done through the
nested ``tag`` type and whether that tag derives from the root tag ``pod``.
Simply, if the Request type's nested ``tag`` type derives from
``boost::network::tags::pod`` then it chooses to enforce the `Pod Server Request
Concept`_, otherwise it chooses the `Normal Client Request Concept`_.

Normal Client Request Concept
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

A type models the Normal Client Request Concept if it models the `Message 
Concept`_ and also supports the following constructs.

**Legend**

:R: The request type.
:r: An instance of R.
:S: The string type.
:s: An instance of S.
:P: The port type.
:p: An instance of P.

+-----------------------+-------------+----------------------------------------+
| Construct             | Result      | Description                            |
+=======================+=============+========================================+
| ``R::string_type``    | ``S``       | The nested ``string_type`` type.       |
+-----------------------+-------------+----------------------------------------+
| ``R::port_type``      | ``P``       | The nested ``port_type`` type.         |
+-----------------------+-------------+----------------------------------------+
| ``R r(s)``            | **NA**      | Construct a Request with an ``s``      |
|                       |             | provided. This treats ``s`` as the URI |
|                       |             | to where the request is destined for.  |
+-----------------------+-------------+----------------------------------------+
| ``host(request)``     | Convertible | Return the host to where the request   |
|                       | to ``S``    | is destined for.                       |
+-----------------------+-------------+----------------------------------------+
| ``port(request)``     | Convertible | Return the port to where the request   |
|                       | to ``P``    | is destined for.                       |
+-----------------------+-------------+----------------------------------------+
| ``path(request)``     | Convertible | Return the path included in the URI.   |
|                       | to ``S``    |                                        |
+-----------------------+-------------+----------------------------------------+
| ``query(request)``    | Convertible | Return the query part of the URI.      |
|                       | to ``S``    |                                        |
+-----------------------+-------------+----------------------------------------+
| ``anchor(request)``   | Convertible | Return the anchor part of the URI.     |
|                       | to ``S``    |                                        |
+-----------------------+-------------+----------------------------------------+
| ``protocol(request)`` | Convertible | Return the protocol/scheme part of the |
|                       | to ``S``    | URI.                                   |
+-----------------------+-------------+----------------------------------------+
| ``r << uri(s)``       | ``R&``      | Set the URI of the request.            |
+-----------------------+-------------+----------------------------------------+
| ``uri(r, s)``         | ``void``    | Set the URI of the request.            |
+-----------------------+-------------+----------------------------------------+



Directives
----------

Modifiers
---------

Wrappers
--------
