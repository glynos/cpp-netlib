// Copyright 2011 Dean Michael Berris <dberris@google.com>.
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef NETWORK_PROTOCOL_HTTP_IMPL_ACCESS_HPP_20111202
#define NETWORK_PROTOCOL_HTTP_IMPL_ACCESS_HPP_20111202

#include <future>
#include <map>
#include <boost/cstdint.hpp>

namespace network {
namespace http { 

struct response;

namespace impl {

struct setter_access {
  void set_version_promise(response &r, std::promise<std::string> &p);
  void set_status_promise(response &r, std::promise<boost::uint16_t> &p);
  void set_status_message_promise(response &r, std::promise<std::string>&p);
  void set_headers_promise(response &r, std::promise<std::multimap<std::string, std::string> > &p);
  void set_source_promise(response &r, std::promise<std::string> &p);
  void set_destination_promise(response &r, std::promise<std::string> &p);
  void set_body_promise(response &r, std::promise<std::string> &p);
};

}  // namespace impl

}  // namespace http

}  // namespace network

#endif  // NETWORK_PROTOCOL_HTTP_IMPL_ACCESS_HPP_20111202
