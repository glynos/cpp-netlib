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
  request_pimpl() {}

  explicit request_pimpl(std::string const & url)
  : uri_(url)
  {}

  request_pimpl* clone() {
    return new (std::nothrow) request_pimpl(*this);
  }

  void set_uri(std::string const & uri) {
    uri_ = uri;
  }

  void get_uri(std::string &uri) {
    uri = uri_.string();
  }

 private:
  uri::uri uri_;
  request_pimpl(request_pimpl const &other)
  : uri_(other.uri_)
  {}
};

request::~request() {
  // do nothing here
}

request::request()
: pimpl_(new (std::nothrow) request_pimpl())
{}

request::request(std::string const & url)
: pimpl_(new (std::nothrow) request_pimpl(url))
{}

request::request(request const &other)
: pimpl_(other.pimpl_->clone())
{}

request& request::operator=(request rhs) {
  rhs.swap(*this);
}

void request::swap(request & other) {
  std::swap(this->pimpl_, other.pimpl_);
}

// From message_base...
// Mutators
void request::set_destination(std::string const & destination){}
void request::set_source(std::string const & source){}
void request::append_header(std::string const & name,
                           std::string const & value){}
void request::remove_headers(std::string const & name){}
void request::remove_headers(){}
void request::set_body(std::string const & body){}
void request::append_body(std::string const & data){}

// Retrievers
void request::get_destination(std::string & destination) const{}
void request::get_source(std::string & source) const{}
void request::get_headers(function<void(std::string const &, std::string const &)> inserter) const{}
void request::get_headers(std::string const & name, function<void(std::string const &, std::string const &)> inserter) const{}
void request::get_headers(function<bool(std::string const &, std::string const &)> predicate, function<void(std::string const &, std::string const &)> inserter) const{}
void request::get_body(std::string & body) const{}
void request::get_body(function<void(iterator_range<char const *>)> chunk_reader, size_t size) const{}

// From request_base...
// Setters
void request::set_method(std::string const & method){}
void request::set_status(std::string const & status){}
void request::set_status_message(std::string const & status_message){}
void request::set_body_writer(function<void(char*, size_t)> writer){}
void request::set_uri(std::string const &uri) {
  pimpl_->set_uri(uri);
}
void request::set_uri(network::uri::uri const &uri){}

// Getters
void request::get_uri(network::uri::uri &uri) const{}

void request::get_uri(std::string &uri) const {
  pimpl_->get_uri(uri);
}

void request::get_method(std::string & method) const{}
void request::get_status(std::string & status) const{}
void request::get_status_message(std::string & status_message) const{}
void request::get_body(function<void(char*, size_t)> chunk_reader) const{}
void request::get_body(std::string const & body) const{}

}  // namespace http

}  // namespace network

}  // namespace boost

#endif /* BOOST_NETWORK_PROTOCOL_HTTP_REQUEST_IPP_20110910 */
