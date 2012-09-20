// Copyright 2011 Dean Michael Berris (dberris@google.com).
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef NETWORK_PROTOCOL_HTTP_RESPONSE_RESPONSE_HPP_20111021
#define NETWORK_PROTOCOL_HTTP_RESPONSE_RESPONSE_HPP_20111021

#include <network/protocol/http/impl/access.hpp>
#include <network/protocol/http/parser/incremental.hpp>

namespace network { namespace http {

struct response_pimpl;

struct response : response_base {
  response();
  response(response const &);
  response& operator=(response);
  void swap(response &);
  bool equals(response const &) const;

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
      boost::function<void(std::string const &, std::string const &)> inserter) const;
  virtual void get_headers(
      std::string const & name,
      boost::function<void(std::string const &, std::string const &)> inserter) const;
  virtual void get_headers(
      boost::function<bool(std::string const &, std::string const &)> predicate,
      boost::function<void(std::string const &, std::string const &)> inserter) const;
  virtual void get_body(std::string & body) const;
  virtual void get_body(
      boost::function<void(boost::iterator_range<char const *>)> chunk_reader,
      size_t size) const;

  // From response_base...
  virtual void set_status(boost::uint16_t new_status);
  virtual void set_status_message(std::string const & new_status_message);
  virtual void set_version(std::string const & new_version);
  virtual void get_status(boost::uint16_t &status) const;
  virtual void get_status_message(std::string &status_message) const;
  virtual void get_version(std::string &version) const;
  virtual ~response();

 private:
  friend struct impl::setter_access;  // Hide access through accessor class.
  // These methods are unique to the response type which will allow for creating
  // promises that can be set appropriately.
  void set_version_promise(boost::promise<std::string>&);
  void set_status_promise(boost::promise<boost::uint16_t>&);
  void set_status_message_promise(boost::promise<std::string>&);
  void set_headers_promise(boost::promise<std::multimap<std::string, std::string> >&);
  void set_source_promise(boost::promise<std::string>&);
  void set_destination_promise(boost::promise<std::string>&);
  void set_body_promise(boost::promise<std::string>&);

  response_pimpl *pimpl_;
};

inline void swap(response &l, response &r) {
  l.swap(r);
}

inline bool operator==(response const &l, response const &r) {
  return l.equals(r);
}

inline bool operator!=(response const &l, response const &r) {
  return !l.equals(r);
}

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

#endif /* NETWORK_PROTOCOL_HTTP_RESPONSE_RESPONSE_HPP_20111021 */
