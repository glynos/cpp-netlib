
//          Copyright Dean Michael Berris 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef __NETWORK_PROTOCOL_HTTP_RESPONSE_20070908_1_HPP__
#define __NETWORK_PROTOCOL_HTTP_RESPONSE_20070908_1_HPP__

#include <boost/network/message.hpp>

namespace boost { namespace network { namespace http {

    template <class tag>
    struct basic_response : public basic_message<tag> {
        typedef typename basic_message<tag>::headers_container_type headers_container_type;
    };

    typedef basic_response<tags::default_> response;

}; // namespace http

}; // namespace network 

}; // namespace boost

#endif // __NETWORK_PROTOCOL_HTTP_RESPONSE_20070908-1_HPP__
