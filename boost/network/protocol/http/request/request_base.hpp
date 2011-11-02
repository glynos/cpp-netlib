#ifndef BOOST_NETWORK_PROTOCOL_HTTP_REQUEST_BASE_HPP_20111008
#define BOOST_NETWORK_PROTOCOL_HTTP_REQUEST_BASE_HPP_20111008

// Copyright 2011 Dean Michael Berris <dberris@google.com>.
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_NETWORK_BUFFER_CHUNK
#define BOOST_NETWORK_BUFFER_CHUNK 1024  // We want 1KiB worth of data at least.
#endif

#include <boost/network/message/message_base.hpp>
#include <boost/network/uri/uri.hpp>
#include <boost/iostreams/categories.hpp>

namespace boost { namespace network { namespace http {

struct body_source : iostreams::source {
  virtual std::streamsize read(char * buffer, std::streamsize size);
  virtual ~body_source();
};

struct request_storage_base {
  typedef iostreams::stream<body_source> body_stream;
 protected:
  // TODO Implement a segmented storage base which efficiently supports
  // efficient memory usage that makes sense for HTTP payload. The idea is
  // to expose the internal (segmented) storage to the client implementation
  // so that raw buffers of formatted HTTP request data (which may or may not
  // support delegated streams from user input -- i.e. for the body contents)
  // can be efficiently sent out to the wire. This also implies that all
  // thread-safety guarantees are handled by the storage base as well.
  request_storage_base(size_t hint = BOOST_NETWORK_BUFFER_CHUNK);
  virtual void set_status_line(std::string const &status_line);
  virtual void append_header(std::string const &name, std::string const &value);
  virtual ~request_storage_base();
};

struct request_base : message_base, request_storage_base {
  using request_storage_base::body_stream;
  // Setters
  virtual void set_method(std::string const & method) = 0;
  virtual void set_status(std::string const & status) = 0;
  virtual void set_status_message(std::string const & status_message) = 0;
  virtual void set_body_stream(shared_ptr<body_stream> stream) = 0;
  virtual void set_uri(std::string const &uri) = 0;
  virtual void set_uri(network::uri::uri const &uri) = 0;

  // Getters
  virtual void get_uri(network::uri::uri &uri) = 0;
  virtual void get_uri(std::string &uri) = 0;
  virtual void get_method(std::string & method) = 0;
  virtual void get_status(std::string & status) = 0;
  virtual void get_status_message(std::string & status_message) = 0;
  virtual void get_body_stream(body_stream & output_stream) = 0;
  virtual ~request_base() = 0;
};

} /* http */
  
} /* network */
  
} /* boost */

#ifdef BOOST_NETWORK_NO_LIB
#include <boost/network/protocol/http/request/request_base.ipp>
#endif

#endif /* BOOST_NETWORK_PROTOCOL_HTTP_REQUEST_BASE_HPP_20111008 */
