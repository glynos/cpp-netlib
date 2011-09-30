#ifndef BOOST_NETWORK_PROTOCOL_HTTP_REQUEST_IPP_20110910
#define BOOST_NETWORK_PROTOCOL_HTTP_REQUEST_IPP_20110910

// Copyright 2011 Dean Michael Berris <dberris@google.com>.
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_NETWORK_BUFFER_CHUNK
#define BOOST_NETWORK_BUFFER_CHUNK 1024  // We want 1KiB worth of data at least.
#endif

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
};

struct request_base : message_base, request_storage_base {
 protected:
  using request_storage_base::body_stream;
  request_base();
  // Setters
  virtual void set_method(std::string const & method);
  virtual void set_status(std::string const & status);
  virtual void set_status_message(std::string const & status_message);
  virtual void append_header(std::string const &name, std::string const &value);
  virtual void set_body_stream(shared_ptr<body_stream> stream);

  // Getters
  virtual void get_method(std::string & method);
  virtual void get_status(std::string & status);
  virtual void get_status_message(std::string & status_message);
  virtual void get_headers(function<void(std::string const &key,std::string const &value) inserter);
  virtual void get_header(std::string const &key, std::string &value);
  virtual void get_body_stream(body_stream & output_stream);
  virtual ~request_base();
} /* http */
} /* network */
} /* boost */

template <class String>
struct basic_request : request_base {
  basic_request();
  basic_request(basic_request const &);  // valid copy constructor
  // TODO implement a conditional move constructor
  basic_request & operator=(basic_request);  // valid assigment operator
  String const method();
  void method(String const &);

  String const status();
  void status(String const &);

  std::multimap<String, String> const & headers();
  
  String const body();
  body_stream & body_stream();
};


#endif /* BOOST_NETWORK_PROTOCOL_HTTP_REQUEST_IPP_20110910 */
