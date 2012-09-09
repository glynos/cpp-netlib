#ifndef BOOST_NETWORK_PROTOCOL_HTTP_IMPL_ACCESS_IPP_20111202
#define BOOST_NETWORK_PROTOCOL_HTTP_IMPL_ACCESS_IPP_20111202

#include <boost/network/protocol/http/request.hpp>

namespace boost { namespace network { namespace http { namespace impl {

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

}  // namespace boost

#endif  // BOOST_NETWORK_PROTOCOL_HTTP_IMPL_ACCESS_IPP_20111202
