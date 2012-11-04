// Copyright 2011 Dean Michael Berris &lt;dberris@google.com&gt;.
// Copyright 2012 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifdef BUILD_SHARED_LIBS
# define BOOST_TEST_DYN_LINK
#endif
#define BOOST_TEST_MODULE HTTP 1.1 Get Streaming Test
#include <network/include/http/client.hpp>
#include <boost/test/unit_test.hpp>
#include <iostream>

namespace net = network;
namespace http = network::http;

struct body_handler {

    explicit body_handler(std::string & body)
    : body(body) {}

    NETWORK_HTTP_BODY_CALLBACK(operator(), range, error) {
        body.append(boost::begin(range), boost::end(range));
    }

    std::string & body;

};


BOOST_AUTO_TEST_CASE(http_client_get_streaming_test) {
  http::client::request request("http://www.boost.org");
  http::client::response response;
  std::string body_string;
  std::string dummy_body;
  body_handler handler_instance(body_string);
  {
      http::client client_;
      BOOST_CHECK_NO_THROW( response = client_.get(request, handler_instance) );
      net::headers_wrapper::container_type const & headers_ = headers(response);
      BOOST_CHECK ( !boost::empty(headers_) );
      BOOST_CHECK_EQUAL ( body(response).size(), 0u );
      std::string version_, status_message_;
      boost::uint16_t status_;
      version_ = version(response);
      status_ = status(response);
      status_message_ = status_message(response);
      BOOST_CHECK_EQUAL ( version_.substr(0, 7), std::string("HTTP/1.") );
      BOOST_CHECK_EQUAL ( status_, 200u );
      BOOST_CHECK_EQUAL ( status_message_, std::string("OK") );
      dummy_body = body(response);
  }
  BOOST_CHECK ( dummy_body == std::string() );
}

