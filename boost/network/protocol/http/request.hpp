#ifndef __NETWORK_PROTOCOL_HTTP_REQUEST_20070908_1_HPP__
#define __NETWORK_PROTOCOL_HTTP_REQUEST_20070908_1_HPP__

// Copyright Dean Michael Berris 2007.
// Copyright 2011 Dean Michael Berris <dberris@google.com>.
// Copyright 2011 Google, Inc. 
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

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
#include <boost/network/protocol/http/message/wrappers/method.hpp>
#include <boost/network/protocol/http/message/directives/method.hpp>
#include <boost/network/protocol/http/message/directives/major_version.hpp>
#include <boost/network/protocol/http/message/directives/minor_version.hpp>
#include <boost/network/protocol/http/message/modifiers/method.hpp>
#include <boost/network/protocol/http/message/modifiers/major_version.hpp>
#include <boost/network/protocol/http/message/modifiers/minor_version.hpp>
#include <boost/network/protocol/http/message/modifiers/clear_headers.hpp>
#include <boost/network/protocol/http/message/wrappers/major_version.hpp>
#include <boost/network/protocol/http/message/wrappers/minor_version.hpp>

#include <boost/network/message/directives.hpp>
#include <boost/network/message/transformers.hpp>

// forward declarations
namespace boost { namespace network { namespace http {

struct request_base;

template <class String>
struct basic_request;

} // namespace http

} // namespace network

} // namespace boost

#include <boost/network/protocol/http/request.ipp>

namespace boost { namespace network { namespace http {

template <class Directive>
request_base & operator<< (request_base & request,
                           Directive const & directive) {
    directive(request);
    return request;
}

} // namespace http

} // namespace network

} // namespace boost

#include <boost/network/protocol/http/request_concept.hpp>

#endif // __NETWORK_PROTOCOL_HTTP_REQUEST_20070908-1_HPP__

