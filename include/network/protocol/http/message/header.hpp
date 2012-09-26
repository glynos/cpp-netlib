//
// header.hpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2008 Christopher M. Kohlhoff (chris at kohlhoff dot com)
// Copyright (c) 2009,2010 Dean Michael Berris (dberris@google.com)
// Copyright (c) 2009 Tarroo, Inc.
//
// Copyright 2012 Google, Inc.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NETWORK_PROTOCOL_HTTP_MESSAGE_HEADER_HPP_20101122
#define NETWORK_PROTOCOL_HTTP_MESSAGE_HEADER_HPP_20101122

#include <string>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/assign/list_of.hpp>

namespace network {
namespace http {

struct request_header {
  std::string name, value;
};

inline void swap(request_header & l, request_header & r) {
  swap(l.name, r.name);
  swap(l.value, r.value);
}

struct response_header {
  std::string name, value;
};

inline void swap(response_header & l, response_header & r) {
  std::swap(l.name, r.name);
  std::swap(l.value, r.value);
}

}  // namespace http
}  // namespace network

BOOST_FUSION_ADAPT_STRUCT(
  network::http::request_header,
  (std::string, name)
  (std::string, value)
  )

BOOST_FUSION_ADAPT_STRUCT(
  network::http::response_header,
  (std::string, name)
  (std::string, value)
  )

#endif // NETWORK_PROTOCOL_HTTP_MESSAGE_HEADER_HPP_20101122
