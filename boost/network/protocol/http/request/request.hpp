#ifndef BOOST_NETWORK_PROTOCOL_HTTP_REQUEST_REQUEST_HPP_20111021
#define BOOST_NETWORK_PROTOCOL_HTTP_REQUEST_REQUEST_HPP_20111021

// Copyright 2011 Dean Michael Berris (dberris@google.com).
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/protocol/http/request/request_base.hpp>

namespace boost { namespace network { namespace http {

struct request_pimpl;

struct request : request_base {
  // FIXME Implement all these!

  // From message_base...
  // Mutators
  virtual void set_destination(std::string const & destination);
  virtual void set_source(std::string const & source);
  virtual void append_header(std::string const & name,
                             std::string const & value);
  virtual void remove_headers(std::string const & name);
  virtual void remove_headers();
  virtual void set_body(std::string const & body);
  virtual void append_body(std::string const & data);

  // Retrievers
  virtual void get_destination(std::string & destination) const;
  virtual void get_source(std::string & source) const;
  virtual void get_headers(function<void(std::string const &, std::string const &)> inserter) const;
  virtual void get_headers(std::string const & name, function<void(std::string const &, std::string const &)> inserter) const;
  virtual void get_headers(function<bool(std::string const &, std::string const &)> predicate, function<void(std::string const &, std::string const &)> inserter) const;
  virtual void get_body(std::string & body) const;
  virtual void get_body(function<void(iterator_range<char const *>)> chunk_reader, size_t size) const;

  // From request_base...
  // Setters
  virtual void set_method(std::string const & method);
  virtual void set_status(std::string const & status);
  virtual void set_status_message(std::string const & status_message);
  virtual void set_body_stream(shared_ptr<body_stream> stream);
  virtual void set_uri(std::string const &uri);
  virtual void set_uri(network::uri::uri const &uri);

  // Getters
  virtual void get_uri(network::uri::uri &uri) const;
  virtual void get_uri(std::string &uri) const;
  virtual void get_method(std::string & method) const;
  virtual void get_status(std::string & status) const;
  virtual void get_status_message(std::string & status_message) const;
  virtual void get_body_stream(body_stream & output_stream) const;

  virtual ~request();
 private:
  request_pimpl * pimpl_;
};

template <class Directive>
request_base & operator<< (request_base & request,
                           Directive const & directive) {
    directive(request);
    return request;
}

} // namespace http

} // namespace network

} // namespace boost

#ifdef BOOST_NETWORK_NO_LIB
#include <boost/network/protocol/http/request/request.ipp>
#endif


#endif /* BOOST_NETWORK_PROTOCOL_HTTP_REQUEST_REQUEST_HPP_20111021 */
