#ifndef BOOST_NETWORK_PROTOCOL_HTTP_REQUEST_IPP_20110910
#define BOOST_NETWORK_PROTOCOL_HTTP_REQUEST_IPP_20110910

// Copyright 2011 Dean Michael Berris <dberris@google.com>.
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/protocol/http/request/request.hpp>
#include <boost/network/protocol/http/request/request_concept.hpp>

#ifdef BOOST_NETWORK_DEBUG
BOOST_CONCEPT_ASSERT((boost::network::http::ClientRequest<boost::network::http::request>));
#endif

namespace boost { namespace network { namespace http {

struct request_pimpl {
  explicit request_pimpl(std::string const & url) {}
};

request::~request() {
  // do nothing here
}

request::request(std::string const & url)
: pimpl_(new request_pimpl(url))
{}


}  // namespace http

}  // namespace network

}  // namespace boost

#endif /* BOOST_NETWORK_PROTOCOL_HTTP_REQUEST_IPP_20110910 */
