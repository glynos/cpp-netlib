#ifndef BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_FACADE_HPP_20100623
#define BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_FACADE_HPP_20100623

// Copyright Dean Michael Berris 2010.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/protocol/http/request.hpp>
#include <boost/network/protocol/http/response.hpp>
#include <boost/network/protocol/http/client/base.hpp>
#include <boost/network/protocol/http/client/parameters.hpp>

namespace boost { namespace network { namespace http {

struct basic_client_facade {
  typedef client_base::body_callback_function_type body_callback_function_type;

  template <class ArgPack>
  basic_client_facade(ArgPack const & args) {
    init_base(args,
               typename mpl::if_<
                 is_same<
                   typename parameter::value_type<ArgPack, tag::io_service, void>::type,
                   void
                 >,
                 no_io_service,
                 has_io_service
               >::type());
  }

  BOOST_PARAMETER_MEMBER_FUNCTION((response const), head, tag, (required (request,(request const &)))) {
    return base->request_skeleton(request, "HEAD", false, body_callback_function_type());
  }

  BOOST_PARAMETER_MEMBER_FUNCTION((response const), get , tag,
                                  (required
                                   (request,(request const &))
                                   )
                                  (optional
                                   (body_handler,(body_callback_function_type),body_callback_function_type())
                                   )) {
    return base->request_skeleton(request, "GET", true, body_handler);
  }

  BOOST_PARAMETER_MEMBER_FUNCTION((response const), post, tag,
                                  (required
                                   (request,(request)) // yes sir, we make a copy of the original request.
                                   )
                                  (optional
                                   (body,(string_type const &),string_type())
                                   (content_type,(string_type const &),string_type())
                                   (body_handler,(body_callback_function_type),body_callback_function_type())
                                   )) {
    if (body != string_type()) {
      request << remove_header("Content-Length")
              << header("Content-Length", boost::lexical_cast<string_type>(body.size()))
              << boost::network::body(body);
    }

    typename headers_range<basic_request<Tag> >::type content_type_headers =
        headers(request)["Content-Type"];
    if (content_type != string_type()) {
      if (!boost::empty(content_type_headers))
        request << remove_header("Content-Type");
      request << header("Content-Type", content_type);
    } else {
      if (boost::empty(content_type_headers)) {
        typedef typename char_<Tag>::type char_type;
        static char_type content_type[] = "x-application/octet-stream";
        request << header("Content-Type", content_type);
      }
    }
    return base->request_skeleton(request, "POST", true, body_handler);
  }

  BOOST_PARAMETER_MEMBER_FUNCTION((response const), put , tag,
                                  (required
                                   (request,(request)) // yes sir, we make a copy of the original request.
                                   )
                                  (optional
                                   (body,(string_type const &),string_type())
                                   (content_type,(string_type const &),string_type())
                                   (body_handler,(body_callback_function_type),body_callback_function_type())
                                   )) {
    if (body != string_type()) {
      request << remove_header("Content-Length")
              << header("Content-Length", boost::lexical_cast<string_type>(body.size()))
              << boost::network::body(body);
    }

    typename headers_range<basic_request<Tag> >::type content_type_headers =
        headers(request)["Content-Type"];
    if (content_type != string_type()) {
      if (!boost::empty(content_type_headers))
        request << remove_header("Content-Type");
      request << header("Content-Type", content_type);
    } else {
      if (boost::empty(content_type_headers)) {
        typedef typename char_<Tag>::type char_type;
        static char_type content_type[] = "x-application/octet-stream";
        request << header("Content-Type", content_type);
      }
    }
    return base->request_skeleton(request, "PUT", true, body_handler);
  }

  BOOST_PARAMETER_MEMBER_FUNCTION((response const), delete_, tag,
                                  (required
                                   (request,(request const &))
                                   )
                                  (optional
                                   (body_handler,(body_callback_function_type),body_callback_function_type())
                                   )) {
    return base->request_skeleton(request, "DELETE", true, body_handler);
  }

  void clear_resolved_cache() {
    base->clear_resolved_cache();
  }

 protected:

  struct no_io_service {};
  struct has_io_service {};

  boost::scoped_ptr<client_base> base;

  template <class ArgPack>
  void init_base(ArgPack const & args, no_io_service) {
    base.reset(new base_type(this->get_connection_manager(args)));
  }

  template <class ArgPack>
  void init_base(ArgPack const & args, has_io_service) {
    base.reset(new base_type(args[_io_service], this->get_connection_manager(args)));
  }

 private:

  template <class ArgPack>
  shared_ptr<connection_manager> get_connection_manager(ArgPack const & args) {
    shared_ptr<connection_manager> connection_manager_ = ptr;
    if (!connection_manager_.get()) {
      // Because there's no explicit connection manager, we use the default
      // implementation as provided by the tag.
      typedef typename async_connection_policy<Tag,version_major,version_minor>::type
          default_connection_manager_type;
      connection_manager_.reset(
          new default_connection_manager_type(args[_cache_resolved|false],
                                              args[_follow_redirects|false],
                                              args[_openssl_certificate|optional<string_type>()],
                                              args[_openssl_verify_path|optional<string_type>()]));
    }

    return connection_manager_;
  }
};

} // namespace http

} // namespace network

} // namespace boost

#endif // BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_FACADE_HPP_20100623
