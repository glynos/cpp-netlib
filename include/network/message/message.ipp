// Copyright 2011 Dean Michael Berris (dberris@google.com).
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef NETWORK_MESSAGE_IPP_20111020
#define NETWORK_MESSAGE_IPP_20111020

#include <iterator>
#include <utility>
#include <algorithm>
#include <network/message/message.hpp>

namespace network {

struct message_pimpl {
  message_pimpl() :
    destination_(),
    source_(),
    headers_(),
    body_(),
    body_read_pos(0)
  {}

  void set_destination(std::string const & destination) {
    destination_ = destination;
  }

  void set_source(std::string const & source) {
    source_ = source;
  }

  void append_header(std::string const & name,
                     std::string const & value) {
    headers_.insert(std::make_pair(name, value));
  }

  void remove_headers(std::string const & name) {
    headers_.erase(name);
  }

  void remove_headers() {
    std::multimap<std::string,std::string>().swap(headers_);
  }

  void set_body(std::string const & body) {
    body_ = body;
  }

  void append_body(std::string const & data) {
    body_.append(data);
  }

  // Retrievers
  void get_destination(std::string & destination) const {
    destination = destination_;
  }

  void get_source(std::string & source) const {
    source = source_;
  }

  void get_headers(std::function<void(std::string const &, std::string const &)> inserter) const {
    std::multimap<std::string, std::string>::const_iterator it  = headers_.begin(),
                                                            end = headers_.end();
    for (; it != end; ++it) inserter(it->first, it->second);
  }

  void get_headers(std::string const & name,
                   std::function<void(std::string const &, std::string const &)> inserter) const {
    std::multimap<std::string, std::string>::const_iterator it = headers_.find(name),
                                                            end= headers_.end();
    while (it != end) {
      inserter(it->first, it->second);
      ++it;
    }
  }

  void get_headers(std::function<bool(std::string const &, std::string const &)> predicate,
                   std::function<void(std::string const &, std::string const &)> inserter) const {
    std::multimap<std::string, std::string>::const_iterator it = headers_.begin(),
                                                            end = headers_.end();
    while (it != end) {
      if (predicate(it->first, it->second))
        inserter(it->first, it->second);
      ++it;
    }
  }

  void get_body(std::string & body) {
    body = body_;
  }

  void get_body(std::function<void(boost::iterator_range<char const *>)> chunk_reader, size_t size) const {
    static char const * nullptr_ = 0;
    if (body_read_pos == body_.size())
      chunk_reader(boost::make_iterator_range(nullptr_, nullptr_));
    std::string::const_iterator it = body_.begin(),
                                end = body_.end();
    std::advance(it, body_read_pos);
    size_t max_size = std::distance(it, end);
    size_t max_read = std::min(max_size, size);
    std::string::const_iterator start = it;
    end = start;
    std::advance(end, max_read);
    body_read_pos += max_read;
    chunk_reader(boost::make_iterator_range(&(*start), &(*end)));
  }

  message_pimpl * clone() {
    message_pimpl * other = new(std::nothrow) message_pimpl;
    other->destination_ = this->destination_;
    other->source_ = this->source_;
    other->headers_ = this->headers_;
    other->body_ = this->body_;
    return other;
  }

 private:
  std::string destination_, source_;
  std::multimap<std::string, std::string> headers_;
  // TODO: use Boost.IOStreams here later on.
  std::string body_;
  mutable size_t body_read_pos;
};

message::message()
  : pimpl(new (std::nothrow) message_pimpl)
{}

message::message(message const & other)
  : pimpl(other.pimpl->clone())
{}

message& message::operator=(message other) {
  *pimpl = *other.pimpl;
  return *this;
}

message::~message() {
  delete pimpl;
}

void message::set_destination(std::string const & destination) {
  pimpl->set_destination(destination);
}

void message::set_source(std::string const & source) {
  pimpl->set_source(source);
}

void message::append_header(std::string const & name,
                            std::string const & value) {
  pimpl->append_header(name, value);
}

void message::remove_headers(std::string const & name) {
  pimpl->remove_headers(name);
}

void message::remove_headers() {
  pimpl->remove_headers();
}

void message::set_body(std::string const & body) {
  pimpl->set_body(body);
}

void message::append_body(std::string const & data) {
  pimpl->append_body(data);
}

void message::get_destination(std::string & destination) const {
  pimpl->get_destination(destination);
}

void message::get_source(std::string & source) const {
  pimpl->get_source(source);
}

void message::get_headers(std::function<void(std::string const &, std::string const &)> inserter) const {
  pimpl->get_headers(inserter);
}

void message::get_headers(std::string const & name,
                          std::function<void(std::string const &, std::string const &)> inserter) const {
  pimpl->get_headers(name, inserter);
}

void message::get_headers(std::function<bool(std::string const &, std::string const &)> predicate,
                          std::function<void(std::string const &, std::string const &)> inserter) const {
  pimpl->get_headers(predicate, inserter);
}

void message::get_body(std::string & body) const {
  pimpl->get_body(body);
}

void message::get_body(std::function<void(boost::iterator_range<char const *>)> chunk_reader, size_t size) const {
  pimpl->get_body(chunk_reader, size);
}

void message::swap(message & other) {
  std::swap(this->pimpl, other.pimpl);
}

} /* network */

#endif /* NETWORK_MESSAGE_IPP_20111020 */
