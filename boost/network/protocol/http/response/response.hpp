#ifndef BOOST_NETWORK_PROTOCOL_HTTP_RESPONSE_RESPONSE_HPP_20111021
#define BOOST_NETWORK_PROTOCOL_HTTP_RESPONSE_RESPONSE_HPP_20111021

// Copyright 2011 Dean Michael Berris (dberris@google.com).
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

namespace boost { namespace network { namespace http {

struct response : response_base {
  // FIXME implement all these!

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
  virtual void get_headers(
      function<void(std::string const &, std::string const &)> inserter) const;
  virtual void get_headers(
      std::string const & name,
      function<void(std::string const &, std::string const &)> inserter) const;
  virtual void get_headers(
      function<bool(std::string const &, std::string const &)> predicate,
      function<void(std::string const &, std::string const &)> inserter) const;
  virtual void get_body(std::string & body) const;
  virtual void get_body(
      function<void(iterator_range<char const *>)> chunk_reader,
      size_t size) const;

  // From response_base...
  virtual void set_status(std::string const & new_status);
  virtual void set_status_message(std::string const & new_status_message);
  virtual void set_version(std::string const & new_version);
  virtual ~response();
};

template <class Directive>
response & operator<<(
    response & message,
    Directive const & directive
    )
{
    directive(message);
    return message;
}

} // namespace http

} // namespace network

} // namespace boost

#endif /* BOOST_NETWORK_PROTOCOL_HTTP_RESPONSE_RESPONSE_HPP_20111021 */
