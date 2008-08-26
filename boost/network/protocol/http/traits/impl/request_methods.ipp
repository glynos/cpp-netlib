
//          Copyright Dean Michael Berris 2008.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_NETWORK_PROTOCOL_HTTP_TRAITS_REQUEST_METHODS_IPP
#define BOOST_NETWORK_PROTOCOL_HTTP_TRAITS_REQUEST_METHODS_IPP

namespace boost { namespace network { namespace http {

    template <>
        struct request_methods<http::message_tag> {
            static char const * const head() {
                static char const * const HEAD = "HEAD";
                return HEAD;
            };
            
            static char const * const get() {
                static char const * const GET = "GET";
                return GET;
            };

            static char const * const put() {
                static char const * const PUT = "PUT";
                return PUT;
            };

            static char const * const post() {
                static char const * const POST = "POST";
                return POST;
            };

            static char const * const delete_() {
                static char const * const DELETE = "DELETE";
                return DELETE;
            };
        };

} // namespace http

} // namespace network

} // namespace boost

#endif // BOOST_NETWORK_PROTOCOL_HTTP_TRAITS_REQUEST_METHODS_IPP

