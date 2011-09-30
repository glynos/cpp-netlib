#ifndef BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_DIRECTIVES_STATUS_HPP_20100603
#define BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_DIRECTIVES_STATUS_HPP_20100603

// Copyright 2010 (c) Sinefunc, Inc.
// Copyright 2011 Dean Michael Berris <dberris@google.com>.
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/thread/future.hpp>
#include <boost/mpl/if.hpp>
#include <boost/cstdint.hpp>
#include <boost/network/protocol/http/response_base.hpp>

namespace boost { namespace network { namespace http {

template <class String>
struct status_directive {

  status_directive(String const & s)
  : status_(s)
  {}

  void operator()(response_base & response) {
    response.set_status(status_);
  }

 protected:
  
  String status_;

};

template <class String>
inline status_directive<String> status(String const & response) {
  return status_directive<String>(response);
}

} // namespace http

} // namespace network

} // namespace boost

#endif // BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_DIRECTIVES_STATUS_HPP_20100603
