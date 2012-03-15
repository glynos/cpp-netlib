#ifndef BOOST_NETWORK_MESSAGE_MESSAGE_HPP_20111021
#define BOOST_NETWORK_MESSAGE_MESSAGE_HPP_20111021

// Copyright 2011 Dean Michael Berris (dberris@google.com).
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <string>
#include <map>
#include <boost/function.hpp>
#include <boost/network/message/message_base.hpp>
#include <boost/shared_container_iterator.hpp>

namespace boost { namespace network {

struct message_pimpl;

/** The common message type.
 */
struct message : message_base {
  // Nested types
  typedef iterator_range<
    std::multimap<std::string, std::string>::const_iterator>
    headers_range;

  // Constructors
  message();
  message(message const & other);

  // Assignment
  message & operator=(message other);

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

  void swap(message & other);

  // Destructor
  virtual ~message();
 private:
  message_pimpl * pimpl;
};

inline void swap(message & left, message & right) {
    left.swap(right);
}

template <class Directive>
message_base & operator<< (message_base & msg, Directive directive) {
  directive(msg);
  return msg;
}

} /* network */

} /* boost */

#ifdef BOOST_NETWORK_NO_LIB
#include <boost/network/message/message.ipp>
#endif

#endif /* BOOST_NETWORK_MESSAGE_MESSAGE_HPP_20111021 */
