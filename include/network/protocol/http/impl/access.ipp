// Copyright 2011 Dean Michael Berris <dberris@google.com>.
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <network/protocol/http/request.hpp>

namespace network {
namespace http {
namespace impl {

void setter_access::set_version_promise(response &r, promise<std::string> &p) {
  return r.set_version_promise(p);
}

void setter_access::set_status_promise(response &r, promise<boost::uint16_t> &p) {
  return r.set_status_promise(p);
}

void setter_access::set_status_message_promise(response &r, promise<std::string> &p) {
  return r.set_status_message_promise(p);
}

void
setter_access::set_headers_promise(response &r, promise<std::multimap<std::string, std::string> > &p) {
  return r.set_headers_promise(p);
}

void setter_access::set_source_promise(response &r, promise<std::string> &p) {
  return r.set_source_promise(p);
}

void setter_access::set_destination_promise(response &r, promise<std::string> &p) {
  return r.set_destination_promise(p);
}

void setter_access::set_body_promise(response &r, promise<std::string> &p) {
  return r.set_body_promise(p);
}

}  // namespace impl
}  // namespace http
}  // namespace network
