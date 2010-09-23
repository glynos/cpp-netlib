
//          Copyright Dean Michael Berris 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef __NETWORK_PROTOCOL_HTTP_REQUEST_20070908_1_HPP__
#define __NETWORK_PROTOCOL_HTTP_REQUEST_20070908_1_HPP__

// Implement the HTTP Request Object

#include <boost/network/tags.hpp>
#include <boost/network/message_fwd.hpp>
#include <boost/network/message/wrappers.hpp>
#include <boost/network/protocol/http/message/directives/uri.hpp>
#include <boost/network/protocol/http/message/modifiers/uri.hpp>
#include <boost/network/protocol/http/message/wrappers/uri.hpp>

#include <boost/network/protocol/http/message/wrappers/host.hpp>
#include <boost/network/protocol/http/message/wrappers/headers.hpp>
#include <boost/network/protocol/http/message/wrappers/path.hpp>
#include <boost/network/protocol/http/message/wrappers/port.hpp>
#include <boost/network/protocol/http/message/wrappers/query.hpp>
#include <boost/network/protocol/http/message/wrappers/anchor.hpp>
#include <boost/network/protocol/http/message/wrappers/protocol.hpp>
#include <boost/network/protocol/http/message/wrappers/body.hpp>
#include <boost/network/protocol/http/message/wrappers/version.hpp>
#include <boost/network/message/directives.hpp>
#include <boost/network/message/transformers.hpp>

#include <boost/network/protocol/http/request_concept.hpp>

// forward declarations
namespace boost { namespace network { namespace http {

    template <class Tag>
    class basic_request;

} // namespace http

} // namespace network

} // namespace boost

#include <boost/network/protocol/http/impl/request.hpp>

namespace boost { namespace network { namespace http {

    template <class Tag, class Directive>
    basic_request<Tag> & operator<<(
        basic_request<Tag> & message,
        Directive const & directive
        )
    {
        directive(message);
        return message;
    }

} // namespace http

} // namespace network

} // namespace boost

#endif // __NETWORK_PROTOCOL_HTTP_REQUEST_20070908-1_HPP__
