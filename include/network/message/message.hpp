// Copyright 2011 Dean Michael Berris (dberris@google.com).
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef NETWORK_MESSAGE_MESSAGE_HPP_20111021
#define NETWORK_MESSAGE_MESSAGE_HPP_20111021

#include <string>
#include <map>
#include <boost/function.hpp>
#include <network/message/message_base.hpp>
#include <boost/shared_container_iterator.hpp>

namespace network {

  struct message_pimpl;

  // The common message type.
  struct message : message_base {
    // Nested types
    typedef boost::iterator_range<
    std::multimap<std::string, std::string>::const_iterator>
    headers_range;

    // Constructors
    message();
    message(message const & other);
    message(message && other) = default;

    // Assignment
    message& operator=(message const &other);
    message& operator=(message &&other);

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
    virtual void get_headers(boost::function<void(std::string const &, std::string const &)> inserter) const;
    virtual void get_headers(std::string const & name,
                             boost::function<void(std::string const &, std::string const &)> inserter) const;
    virtual void get_headers(boost::function<bool(std::string const &, std::string const &)> predicate,
                             boost::function<void(std::string const &, std::string const &)> inserter) const;
    virtual void get_body(std::string & body) const;
    virtual void get_body(boost::function<void(std::string::const_iterator, size_t)> chunk_reader, size_t size) const;

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

}  // namespace network

#endif /* NETWORK_MESSAGE_MESSAGE_HPP_20111021 */
