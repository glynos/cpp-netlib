#ifndef BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_FACADE_IPP_20120303
#define BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_FACADE_IPP_20120303

#include <boost/network/protocol/http/client/facade.hpp>

namespace boost { namespace network { namespace http {

basic_client_facade::basic_client_facade()
: base(new (std::nothrow) client_base())
{}

basic_client_facade::basic_client_facade(client_options const &options)
: base(new (std::nothrow) client_base(options))
{}

response const basic_client_facade::head(request const &request,
                                         request_options const&options) {
  return base->request_skeleton(request,
                                "HEAD",
                                false,
                                body_callback_function_type(),
                                options);
}

response const basic_client_facade::get(request const &request,
                                        body_callback_function_type body_handler,
                                        request_options const &options) {
  return base->request_skeleton(request, "GET", true, body_handler, options);
}

response const basic_client_facade::post(request request,
                                         optional<std::string> body,
                                         optional<std::string> content_type,
                                         body_callback_function_type body_handler,
                                         request_options const &options) {
  if (body) {
    request << remove_header("Content-Length")
            << header("Content-Length", boost::lexical_cast<std::string>(body->size()))
            << boost::network::body(*body);
  }

  headers_wrapper::range_type content_type_headers =
      headers(request)["Content-Type"];
  if (content_type) {
    if (!boost::empty(content_type_headers))
      request << remove_header("Content-Type");
    request << header("Content-Type", *content_type);
  } else {
    if (boost::empty(content_type_headers)) {
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
  if (body) {
    request << remove_header("Content-Length")
            << header("Content-Length", boost::lexical_cast<std::string>(body->size()))
            << boost::network::body(*body);
  }

  headers_wrapper::range_type content_type_headers =
      headers(request)["Content-Type"];
  if (content_type) {
    if (!boost::empty(content_type_headers))
      request << remove_header("Content-Type");
    request << header("Content-Type", *content_type);
  } else {
    if (boost::empty(content_type_headers)) {
      static char default_content_type[] = "x-application/octet-stream";
      request << header("Content-Type", default_content_type);
    }
  }
  return base->request_skeleton(request, "PUT", true, body_handler, options);
}

response const basic_client_facade::delete_(request const & request,
                                            body_callback_function_type body_handler,
                                            request_options const & options) {
  return base->request_skeleton(request, "DELETE", true, body_handler, options);
}

void basic_client_facade::clear_resolved_cache() {
  base->clear_resolved_cache();
}

}  // namespace http

}  // namespace network

}  // namespace boost


#endif  // BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_FACADE_IPP_20120303

