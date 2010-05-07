//            Copyright (c) Glyn Matthews 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#ifndef __BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_RESPONSE_CONCEPT_INC__
# define __BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_RESPONSE_CONCEPT_INC__


namespace boost {
namespace network {
namespace http {
template <
    class R
    >
struct Response
    : Message<R> {

    BOOST_CONCEPT_USAGE(Response) {

        // version
        // status
        // message
    }

private:

    R response;
};
} // namespace http
} // namespace network
} // namespace boost


#endif // __BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_RESPONSE_CONCEPT_INC__
