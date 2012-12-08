// Copyright 2011 Dean Michael Berris <dberris@google.com>. 
// Copyright 2012 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef NETWORK_PROTOCOL_HTTP_CLIENT_MACROS_HPP_20110430
#define NETWORK_PROTOCOL_HTTP_CLIENT_MACROS_HPP_20110430

#include <boost/range/iterator_range.hpp>
#include <boost/asio/placeholders.hpp>

#ifndef NETWORK_HTTP_BODY_CALLBACK
#define NETWORK_HTTP_BODY_CALLBACK(function_name, range_name, error_name) \
        void function_name (boost::iterator_range<const char*> const & range_name, boost::system::error_code const & error_name)
#endif

#endif /* NETWORK_PROTOCOL_HTTP_CLIENT_MACROS_HPP_20110430 */

