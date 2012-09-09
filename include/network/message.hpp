#ifndef BOOST_NETWORK_MESSAGE_HPP_20111021
#define BOOST_NETWORK_MESSAGE_HPP_20111021

// Copyright Dean Michael Berris 2007.
// Copyright 2011 Dean Michael Berris (dberris@google.com).
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/message_fwd.hpp>
#include <boost/network/detail/directive_base.hpp>
#include <boost/network/detail/wrapper_base.hpp>
#include <boost/network/message/directives.hpp>
#include <boost/network/message/wrappers.hpp>
#include <boost/network/message/transformers.hpp>

#include <boost/network/message/modifiers/add_header.hpp>
#include <boost/network/message/modifiers/remove_header.hpp>
#include <boost/network/message/modifiers/clear_headers.hpp>
#include <boost/network/message/modifiers/source.hpp>
#include <boost/network/message/modifiers/destination.hpp>
#include <boost/network/message/modifiers/body.hpp>

#include <boost/network/message/message.hpp>

#ifdef BOOST_NETWORK_DEBUG
#include <boost/network/message/message_concept.hpp>
#endif

#endif // BOOST_NETWORK_MESSAGE_HPP_20111021
