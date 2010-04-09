//            Copyright (c) Glyn Matthews 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#ifndef __BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_REQUEST_CONCEPT_INC__
# define __BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_REQUEST_CONCEPT_INC__


# include <boost/concept_check.hpp>
# include <boost/network/message/message_concept.hpp>


namespace boost {
namespace network {
namespace http {
template <
    class R
    >
struct Request
    : Message<R> {
    typedef typename R::string_type string_type;

    BOOST_CONCEPT_USAGE(Request) {

        string_type uri = request.uri();
        string_type method = request.method();
        string_type http_version_major = request.http_version_major();
        string_type http_version_minor = request.http_version_minor();

        (void)uri;
        (void)method;
        (void)http_version_major;
        (void)http_version_minor;
    }

private:
    
    R request;
};
} // namespace http
} // namespace network
} // namespace boost


#endif // __BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_REQUEST_CONCEPT_INC__
