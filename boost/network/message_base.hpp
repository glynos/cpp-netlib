#ifndef BOOST_NETWORK_MESSAGE_BASE_HPP_20110910
#define BOOST_NETWORK_MESSAGE_BASE_HPP_20110910

// Copyright 2011 Dean Michael Berris <dberris@google.com>.
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

namespace boost { namespace network {

struct message_base {
  // Mutators
  virtual void set_destination(std::string const & destination) = 0;
  virtual void set_source(std::string const & source) = 0;
  virtual void append_header(std::string const & name,
                             std::string const & value) = 0;
  virtual void remove_headers(std::string const & name) = 0;
  virtual void set_body(std::string const & body) = 0;
  virtual void append_body(std::string const & data) = 0;

  // Retrievers
  virtual void get_destination(std::string & destination) = 0;
  virtual void get_source(std::string & source) = 0;
  virtual void get_headers(function<void(std::string const &, std::string const &)> inserter) = 0;
  virtual void get_headers(std::string const & name, function<void(std::string const &, std::string const &)> inserter) = 0;
  virtual void get_headers(function<bool(std::string const &, std::string const &)> predicate, function<void(std::string const &, std::string const &)> inserter) = 0;
  virtual void get_body(std::string const & body) = 0;
  virtual void get_body(function<void(iterator_range<char const *>)> chunk_reader, size_t size) = 0;

  // Destructor
  virtual ~message_base() = 0;  // pure virtual
};

} /* network */

} /* boost */

#endif /* BOOST_NETWORK_MESSAGE_BASE_HPP_20110910 */
