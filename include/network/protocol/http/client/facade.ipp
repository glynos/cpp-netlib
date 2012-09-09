#ifndef BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_FACADE_IPP_20120303
#define BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_FACADE_IPP_20120303

#include <network/protocol/http/client/facade.hpp>
#include <network/detail/debug.hpp>

namespace boost { namespace network { namespace http {

basic_client_facade::basic_client_facade()
: base(new (std::nothrow) client_base()) {
  BOOST_NETWORK_MESSAGE("basic_client_facade::basic_client_facade()");
}

basic_client_facade::basic_client_facade(client_options const &options)
: base(new (std::nothrow) client_base(options)) {
  BOOST_NETWORK_MESSAGE("basic_client_facade::basic_client_facade(client_options const &)");
}

response const basic_client_facade::head(request const &request,
                                         request_options const&options) {
  BOOST_NETWORK_MESSAGE("basic_client_facade::head(...)");
  return base->request_skeleton(request,
                                "HEAD",
                                false,
                                body_callback_function_type(),
                                options);
}

response const basic_client_facade::get(request const &request,
                                        body_callback_function_type body_handler,
                                        request_options const &options) {
  BOOST_NETWORK_MESSAGE("basic_client_facade::get(...)");
  return base->request_skeleton(request, "GET", true, body_handler, options);
}

response const basic_client_facade::post(request request,
                                         optional<std::string> body,
                                         optional<std::string> content_type,
                                         body_callback_function_type body_handler,
                                         request_options const &options) {
  BOOST_NETWORK_MESSAGE("basic_client_facade::post(...)");
  if (body) {
    BOOST_NETWORK_MESSAGE("using body provided.");
    request << remove_header("Content-Length")
            << header("Content-Length", boost::lexical_cast<std::string>(body->size()))
            << boost::network::body(*body);
  }

  headers_wrapper::container_type const & headers_ =
      headers(request);
  if (content_type) {
    BOOST_NETWORK_MESSAGE("using provided content type.");
    request << remove_header("Content-Type")
            << header("Content-Type", *content_type);
  } else {
    BOOST_NETWORK_MESSAGE("using default content type.");
    if (boost::empty(headers_.equal_range("Content-Type"))) {
      static char default_content_type[] = "x-application/octet-stream";
      request << header("Content-Type", default_content_type);
    }
  }
  return base->request_skeleton(request, "POST", true, body_handler, options);
}

response const basic_client_facade::put(request request,
                                        optional<std::string> body,
                                        optional<std::string> content_type,
                                        body_callback_function_type body_handler,
                                        request_options const & options) {
  BOOST_NETWORK_MESSAGE("basic_client_facade::put(...)");
  if (body) {
    BOOST_NETWORK_MESSAGE("using body provided.");
    request << remove_header("Content-Length")
            << header("Content-Length", boost::lexical_cast<std::string>(body->size()))
            << boost::network::body(*body);
  }

  headers_wrapper::container_type const & headers_ =
      headers(request);
  if (content_type) {
    BOOST_NETWORK_MESSAGE("using provided content type.");
    request << remove_header("Content-Type")
            << header("Content-Type", *content_type);
  } else {
    BOOST_NETWORK_MESSAGE("using default content type.");
    if (boost::empty(headers_.equal_range("Content-Type"))) {
      static char default_content_type[] = "x-application/octet-stream";
      request << header("Content-Type", default_content_type);
    }
  }
  return base->request_skeleton(request, "PUT", true, body_handler, options);
}

response const basic_client_facade::delete_(request const & request,
                                            body_callback_function_type body_handler,
                                            request_options const & options) {
  BOOST_NETWORK_MESSAGE("basic_client_facade::delete_(...)");
  return base->request_skeleton(request, "DELETE", true, body_handler, options);
}

void basic_client_facade::clear_resolved_cache() {
  BOOST_NETWORK_MESSAGE("basic_client_facade::clear_resolved_cache()");
  base->clear_resolved_cache();
}

}  // namespace http

}  // namespace network

}  // namespace boost


#endif  // BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_FACADE_IPP_20120303

