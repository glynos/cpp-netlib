//          Copyright Dean Michael Berris 2007.
//          Copyright Michael Dickey 2008.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_NETWORK_PROTOCOL_HTTP_RESPONSE_HPP
#define BOOST_NETWORK_PROTOCOL_HTTP_RESPONSE_HPP

#include <boost/cstdint.hpp>

#include <boost/network/protocol/http/response/response_base.hpp>
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

#include <boost/network/protocol/http/response/response.hpp>
#include <boost/network/protocol/http/response/response_concept.hpp>

#endif // BOOST_NETWORK_PROTOCOL_HTTP_RESPONSE_HPP
