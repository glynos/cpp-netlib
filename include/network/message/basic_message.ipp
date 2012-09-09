#ifndef BOOST_NETWORK_MESSAGE_BASIC_MESSAGE_IPP_20110911
#define BOOST_NETWORK_MESSAGE_BASIC_MESSAGE_IPP_20110911

// Copyright 2011 Dean Michael Berris <dberris@google.com>.
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

namespace boost { namespace network {

struct basic_storage_pimpl {
  basic_storage_pimpl();
  basic_storage_pimpl(basic_storage_pimpl const &);

  virtual basic_storage_pimpl* clone();
 protected:
  friend struct basic_storage_base;
  std::string source_, destination_;
  typedef std::multimap<std::string, std::string> headers_container_type;
  headers_container_type headers_;
  std::string body_;
};

basic_storage_base::basic_storage_base()
: pimpl(new (std::nothrow) basic_storage_pimpl())
{}

basic_storage_base::basic_storage_base(basic_storage_base const & other)
: pimpl(other.clone())
{}

void basic_storage_base::set_destination(std::string const & destination) {
  pimpl->destination_ = destination;
}

void basic_storage_base::set_source(std::string const & source) {
  pimpl->source_ = source;
}

void basic_storage_base::append_header(std::string const & name,
                                       std::string const & value) {
  pimpl->headers_.insert(std::make_pair(name, value));
}

void basic_storage_base::remove_headers(std::string const & name) {
  pimpl->headers_.erase(name);
}

void basic_storage_base::remove_headers() {
  basic_storage_pimpl::headers_container_type().swap(pimpl->headers_);
}

void basic_storage_base::set_body(std::string const & body) {
  pimpl->body = body;
}

void basic_storage_base::append_body(std::string const & data) {
  pimpl->body.append(data);
}

void basic_storage_base::get_destination(std::string & destination) {
  destination = pimpl->destination;
}

void basic_storage_base::get_source(std::string & source) {
  source = pimpl->source;
}

void basic_storage_base::get_headers(function<void(std::string const &, std::string const &)> inserter) {
  copy(pimpl->headers_, inserter);
}

void basic_storage_base::get_headers(std::string const & name, function<void(std::string const &, std::string const &)> inserter) {
  basic_storage_pimpl::headers_container_type::const_iterator
      it = pimpl->headers_.find(name),
      pe = pimpl->headers_.end();
  for (; it != pe; ++it)
    inserter(it->first, it->second);
}

void basic_storage_base::get_body(std::string & body) {
  // TODO use iostreams!
  body = pimpl_->body;
}

void basic_storage_base::get_body(function<void(iterator_range<char const *>)> chunk_reader, size_t size) {
  // TODO use iostreams!
  std::string::const_iterator it = pimpl->body.begin(),
                              pe = pimpl->body.end();
  std::advance(it, size);
  chunk_reader(make_iterator_range(it, pe));
  pimpl->body.assign(it, pe);
}

basic_storage_base::~basic_storage_base() {
  pimpl->reset();
}

void basic_storage_base::swap(basic_storage_base & other) {
  std::swap(pimpl, other.pimpl);
}

void swap(basic_storage_base & l, basic_storage_base & r) {
  l.swap(r);
}

} /* network */
  
} /* boost */

#endif /* BOOST_NETWORK_MESSAGE_BASIC_MESSAGE_IPP_20110911 */
