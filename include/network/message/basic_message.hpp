#ifndef BOOST_NETWORK_MESSAGE_BASIC_MESSAGE_HPP_20110911
#define BOOST_NETWORK_MESSAGE_BASIC_MESSAGE_HPP_20110911

// Copyright 2011 Dean Michael Berris <dberris@google.com>.
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <network/message_base.hpp>
#include <boost/scoped_ptr.hpp>

namespace boost { namespace network {

struct basic_storage_pimpl;

struct basic_storage_base : message_base {
  basic_storage_base();
  basic_storage_base(basic_storage_base const &);
  virtual void set_destination(std::string const & destination);
  virtual void set_source(std::string const & source);
  virtual void append_header(std::string const & name,
                             std::string const & value);
  virtual void remove_headers(std::string const & name);
  virtual void remove_headers();
  virtual void set_body(std::string const & body);
  virtual void append_body(std::string const & data);

  virtual void get_destination(std::string & destination);
  virtual void get_source(std::string & source);
  virtual void get_headers(function<void(std::string const &, std::string const &)> inserter);
  virtual void get_headers(std::string const & name, function<void(std::string const &, std::string const &)> inserter);
  virtual void get_body(std::string & body);
  virtual void get_body(function<void(iterator_range<char const *>)> chunk_reader, size_t size);

  void swap(basic_storage_base & other);

  virtual ~basic_storage_base();
 protected:
  scoped_ptr<basic_storage_pimpl> pimpl;
};

void swap(basic_storage_base & l, basic_storage_base & r);

} /* network */
} /* boost */

#ifdef BOOST_NETWORK_NO_LIB
#include <network/message/basic_message.ipp>
#endif

#endif /* BOOST_NETWORK_MESSAGE_BASIC_MESSAGE_HPP_20110911 */
