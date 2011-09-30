//          Copyright Dean Michael Berris 2007.
//          Copyright Michael Dickey 2008.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_NETWORK_PROTOCOL_HTTP_RESPONSE_HPP
#define BOOST_NETWORK_PROTOCOL_HTTP_RESPONSE_HPP

#include <boost/cstdint.hpp>

struct response_base;

#include <boost/network/protocol/http/message/directives/status_message.hpp>
#include <boost/network/protocol/http/message/directives/version.hpp>
#include <boost/network/protocol/http/message/directives/status.hpp>
#include <boost/network/protocol/http/message/directives/uri.hpp>

#include <boost/network/protocol/http/message/modifiers/uri.hpp>
#include <boost/network/protocol/http/message/modifiers/version.hpp>
#include <boost/network/protocol/http/message/modifiers/status.hpp>
#include <boost/network/protocol/http/message/modifiers/status_message.hpp>

#include <boost/network/protocol/http/message/wrappers/version.hpp>
#include <boost/network/protocol/http/message/wrappers/status.hpp>
#include <boost/network/protocol/http/message/wrappers/status_message.hpp>
#include <boost/network/protocol/http/message/wrappers/ready.hpp>

#include <boost/network/protocol/http/response_concept.hpp>

namespace boost { namespace network { namespace http {

} // namespace http

} // namespace network

} // namespace boost

#include <boost/network/protocol/http/impl/response.ipp>

namespace boost { namespace network { namespace http {

    template <class Tag, class Directive>
    basic_response<Tag> & operator<<(
        basic_response<Tag> & message,
        Directive const & directive
        )
    {
        directive(message);
        return message;
    }

} // namespace http

} // namespace network

} // namespace boost

#endif // BOOST_NETWORK_PROTOCOL_HTTP_RESPONSE_HPP
