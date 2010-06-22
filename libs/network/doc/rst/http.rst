HTTP implementation
===================

HTTP client
```````````

The cpp-netlib HTTP client is composed of three template classes:

.. code-block:: c++

    namespace http {
        template <class Tag> basic_request;
        template <class Tag> basic_response;
        template <class Tag> basic_client;
        typedef basic_request<default_> request;
        typedef basic_response<default_> response;
        typedef basic_client<default_> client;
    }


Each of these use again the tag-based static polymorphism that was described in
previous sections.  A default, human-readable typedef is provided for each one
of the ``basic_request``,  ``basic_response`` and  ``basic_client``.
``basic_request`` and ``basic_response`` each model the message concept. They
make use of directives to set and get HTTP headers, body etc. The code snippet
below shows how to set the HTTP header field "Connection" with the option
"close" using the DSEL described in the directives section:

.. code-block:: c++

    using namespace boost::network;
    http::request request("http://www.boost.org/");
    request << header("Connection", "close");

The ``basic_client`` implements all HTTP methods as member functions (HEAD,
GET, POST, PUT, DELETE).  Therefore, the code to make an HTTP request looks
trivially simple:

.. code-block:: c++

    using namespace boost::network;
    http::client client;
    http::request request("http://www.boost.org/");
    http::response response = client.get(request);

Accessing data from ``http::response`` is also done using directives.  To
get the response headers, we use the ``headers`` directive which returns,
in the default case, a map of strings to strings:

.. code-block:: c++

    using namespace boost::network;
    typedef headers_range<http_client::response>::type response_headers;
    boost::range_iterator<response_headers>::type iterator;

    response_headers headers_ = headers(response);
    for (iterator it = headers_.begin(); it != headers_.end(); ++it) {
        std::cout << it->first << ": " << it->second << std::endl;
    }    
    std::cout << std::endl;

HTTP server
```````````

As with the HTTP client, the HTTP server that is provided with cpp-netlib is
extensible through the tag mechanism and is embeddable.  The template class
declaration of ``basic_server`` is given below:

.. code-block:: c++

    namespace http {
        template <class Tag, class RequestHandler> basic_server;
    }

The second template argument is used to specify the request handler type. The
request handler type is a functor type which should overload the function call
operator (``RequestHandler::operator()`` should be overloaded) that takes two
parameters: the first one being a reference to a ``const basic_request<Tag>``
and the second being a reference to a ``basic_response<Tag>`` instance.

All the logic for parsing the HTTP request and building the ``const
basic_request<Tag>`` object resides internally in the ``basic_server`` template.
Processing the request is delegated to the ``RequestHandler`` type, and the
assumption of which would be that the response is formed inside the
``RequestHandler`` function call operator overload.

The ``basic_server`` template however is only an underlying implementation while
the user-visible implementation is the ``http::server`` template. This simply
specializes the ``basic_server`` template to use the ``default_`` tag and
forwards the ``RequestHandler`` parameter:

.. code-block:: c++

    namespace http {
        template <class RequestHandler> server
        : public basic_server<default_, RequestHandler>
        {};
    }

To use the forwarding server type you just supply the request handler
implementation as the parameter. For example, an "echo" server example might 
look something like this:

.. code-block:: c++

    using namespace boost::network;
    struct echo;
    typedef http::server<echo> echo_server;

    struct echo {
        void operator () (const echo_server::request &request,
                          echo_server::response &response) const {
            response = echo_server::response::stock_reply(
                echo_server::response::ok,
		body(request));
        }
    };


Here, all we're doing is returning the original request body with an HTTP OK
response (200).

HTTP URI
````````

Firstly, cpp-netlib provides a specialization and ``typedef`` for an HTTP URI:

.. code-block:: c++

    namespace http {
        template <class T> class basic_uri;
        typedef basic_uri<default_> uri;
    }
    
``basic_uri`` provides a parser which breaks down a URI string passed to it's
constructor into different parts.

.. code-block:: c++

    using namespace boost::network::uri;
    http::uri uri_("http://www.boost.org/");
    assert(valid(uri_));
    assert(scheme(uri_) == "http");
    assert(host(uri_) == "www.boost.org")
    assert(port(uri_) == 80)
    assert(path(uri_) == "/");

The syntax of the HTTP URI are defined in RFC 1738 section 3.3 [#]_ and the
default URI provided with cpp-netlib conforms to this.  In such a way,
specializations that conform to any URI scheme can be provided.

.. [#] http://tools.ietf.org/html/rfc1738
