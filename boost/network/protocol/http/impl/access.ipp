#ifndef BOOST_NETWORK_PROTOCOL_HTTP_IMPL_ACCESS_IPP_20111202
#define BOOST_NETWORK_PROTOCOL_HTTP_IMPL_ACCESS_IPP_20111202

#include <boost/network/protocol/http/request.hpp>

namespace boost { namespace network { namespace http { namespace impl {

promise<std::string> setter_access::get_version_promise(response &r) {
  return r.get_version_promise();
}

promise<boost::uint16_t> setter_access::get_status_promise(response &r) {
  return r.get_status_promise();
}

promise<std::string> setter_access::get_status_message_promise(response &r) {
  return r.get_status_message_promise();
}

promise<std::multimap<std::string, std::string> >
setter_access::get_headers_promise(response &r) {
  return r.get_headers_promise();
}

promise<std::string> setter_access::get_source_promise(response &r) {
  return r.get_source_promise();
}

promise<std::string> setter_access::get_destination_promise(response &r) {
  return r.get_destination_promise();
}

promise<std::string> setter_access::get_body_promise(response &r) {
  return r.get_body_promise();
}

}  // namespace impl

}  // namespace http

}  // namespace network

}  // namespace boost

#endif  // BOOST_NETWORK_PROTOCOL_HTTP_IMPL_ACCESS_IPP_20111202
