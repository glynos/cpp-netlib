// Copyright Dean Michael Berris 2007.
// Copyright 2011 Dean Michael Berris (dberris@google.com).
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef NETWORK_MESSAGE_HPP_20111021
#define NETWORK_MESSAGE_HPP_20111021

#include <network/message_fwd.hpp>
#include <network/detail/directive_base.hpp>
#include <network/detail/wrapper_base.hpp>
#include <network/message/directives.hpp>
#include <network/message/wrappers.hpp>
#include <network/message/transformers.hpp>

#include <network/message/modifiers/add_header.hpp>
#include <network/message/modifiers/remove_header.hpp>
#include <network/message/modifiers/clear_headers.hpp>
#include <network/message/modifiers/source.hpp>
#include <network/message/modifiers/destination.hpp>
#include <network/message/modifiers/body.hpp>

#include <network/message/message.hpp>

#ifdef NETWORK_DEBUG
#include <network/message/message_concept.hpp>
#endif

#endif // NETWORK_MESSAGE_HPP_20111021
