// Copyright 2011 Dean Michael Berris <dberris@google.com>.
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef NETWORK_PROTOCOL_HTTP_REQUEST_IPP_20110910
#define NETWORK_PROTOCOL_HTTP_REQUEST_IPP_20110910

#include <network/protocol/http/request/request.hpp>
#include <network/protocol/http/request/request_concept.hpp>
#include <boost/scoped_array.hpp>

#ifdef NETWORK_DEBUG
BOOST_CONCEPT_ASSERT((network::http::ClientRequest<network::http::request>));
#endif

namespace network { namespace http {

struct request_pimpl {
  request_pimpl()
  : uri_()
  , read_offset_(0)
  , source_()
  , destination_()
  , headers_()
  {}

  explicit request_pimpl(std::string const & url)
  : uri_(url)
  , read_offset_(0)
  , source_()
  , destination_()
  , headers_()
  {}

  explicit request_pimpl(::network::uri const & url)
  : uri_(url)
  , read_offset_(0)
  , source_()
  , destination_()
  , headers_()
  {}

  request_pimpl* clone() const {
    return new (std::nothrow) request_pimpl(*this);
  }

  void set_uri(std::string const & uri) {
    uri_ = uri;
  }

  void set_uri(::network::uri const & uri) {
    uri_ = uri;
  }

  void get_uri(std::string &uri) {
    uri = uri_.string();
  }

  void get_uri(::network::uri &uri) {
    uri = uri_;
  }

  void append_header(std::string const & name, std::string const & value) {
    headers_.insert(std::make_pair(name, value));
  }

  void get_headers(std::function<bool(std::string const &, std::string const &)> predicate,
                   std::function<void(std::string const &, std::string const &)> inserter) const {
    headers_type::const_iterator it = headers_.begin();
    for (; it != headers_.end(); ++it) {
      if (predicate(it->first, it->second)) {
        inserter(it->first, it->second);
      }
    }
  }

  void get_headers(std::function<void(std::string const &, std::string const &)> inserter) const {
    headers_type::const_iterator it = headers_.begin();
    for (; it != headers_.end(); ++it) {
      inserter(it->first, it->second);
    }
  }

  void get_headers(std::string const &name,
                   std::function<void(std::string const &, std::string const &)> inserter) const {
    headers_type::const_iterator it = headers_.begin();
    for (; it != headers_.end(); ++it) {
      if (it->first == name) {
        inserter(it->first, it->second);
      }
    }
  }

  void set_source(std::string const &source) {
    source_ = source;
  }

  void get_source(std::string &source) const {
    source = source_;
  }

  void set_destination(std::string const &destination) {
    destination_ = destination;
  }

  void get_destination(std::string &destination) const {
    destination = destination_;
  }

  size_t read_offset() const {
    return read_offset_;
  }

  void advance_read_offset(size_t bytes) {
    read_offset_ += bytes;
  }

  bool equals(request_pimpl const &other) const {
    return uri_ == other.uri_ &&
           read_offset_ == other.read_offset_ &&
           source_ == other.source_ &&
           destination_ == other.destination_ &&
           headers_ == other.headers_;
  }

  void set_version_major(unsigned short major_version) {
    version_major_ = major_version;
  }

  void set_version_minor(unsigned short minor_version) {
    version_minor_ = minor_version;
  }

  void get_version_major(unsigned short &major_version) {
    major_version = version_major_;
  }

  void get_version_minor(unsigned short &minor_version) {
    minor_version = version_minor_;
  }

 private:
  typedef std::multimap<std::string, std::string> headers_type;

  ::network::uri uri_;
  size_t read_offset_;
  std::string source_, destination_;
  headers_type headers_;
  unsigned short version_major_, version_minor_;

  request_pimpl(request_pimpl const &other)
  : uri_(other.uri_)
  , read_offset_(other.read_offset_)
  , source_(other.source_)
  , destination_(other.destination_)
  , headers_(other.headers_)
  {}
};

request::~request() {
  // do nothing here
}

request::request()
: pimpl_(new (std::nothrow) request_pimpl())
{}

request::request(std::string const & url)
: pimpl_(new (std::nothrow) request_pimpl(url))
{}

request::request(::network::uri const & url)
: pimpl_(new (std::nothrow) request_pimpl(url))
{}

request::request(request const &other)
: pimpl_(other.pimpl_->clone())
{}

request& request::operator=(request rhs) {
  rhs.swap(*this);
  return *this;
}

bool request::equals(request const &other) const {
  return pimpl_->equals(*other.pimpl_) &&
         request_storage_base::equals(other);
}

void request::swap(request & other) {
  std::swap(this->pimpl_, other.pimpl_);
  request_storage_base::swap(other);
}

// From message_base...
// Mutators
void request::set_destination(std::string const & destination) {
  pimpl_->set_destination(destination);
}

void request::set_source(std::string const & source) {
  pimpl_->set_source(source);
}

void request::append_header(std::string const & name,
                           std::string const & value) {
  pimpl_->append_header(name, value);
}

void request::remove_headers(std::string const & name) {
}

void request::remove_headers() {
}

void request::set_body(std::string const & body) {
  this->clear();
  this->append(body.data(), body.size());
}

void request::append_body(std::string const & data) {
  this->append(data.data(), data.size());
}

// Retrievers
void request::get_destination(std::string & destination) const {
  pimpl_->get_destination(destination);
}

void request::get_source(std::string & source) const {
  pimpl_->get_source(source);
}

void request::get_headers(std::function<void(std::string const &, std::string const &)> inserter) const {
  pimpl_->get_headers(inserter);
}

void request::get_headers(std::string const & name, std::function<void(std::string const &, std::string const &)> inserter) const {
  pimpl_->get_headers(name, inserter);
}

void request::get_headers(std::function<bool(std::string const &, std::string const &)> predicate, std::function<void(std::string const &, std::string const &)> inserter) const {
  pimpl_->get_headers(predicate, inserter);
}

void request::get_body(std::string & body) const {
  this->flatten(body);
}

void request::get_body(std::function<void(boost::iterator_range<char const *>)> chunk_reader, size_t size) const {
  boost::scoped_array<char> local_buffer(new (std::nothrow) char[size]);
  size_t bytes_read = this->read(local_buffer.get(),
                                 pimpl_->read_offset(),
                                 size);
  pimpl_->advance_read_offset(bytes_read);
  char const * begin = local_buffer.get();
  char const * end = local_buffer.get() + bytes_read;
  chunk_reader(boost::make_iterator_range(begin, end));
}

// From request_base...
// Setters
void request::set_method(std::string const & method) {
}

void request::set_status(std::string const & status) {
}

void request::set_status_message(std::string const & status_message) {
}

void request::set_body_writer(std::function<void(char*, size_t)> writer) {
}

void request::set_uri(std::string const &uri) {
  pimpl_->set_uri(uri);
}

void request::set_uri(::network::uri const &uri) {
  pimpl_->set_uri(uri);
}

void request::set_version_major(unsigned short major_version) {
  pimpl_->set_version_major(major_version);
}

void request::set_version_minor(unsigned short minor_version) {
  pimpl_->set_version_minor(minor_version);
}

// Getters
void request::get_uri(::network::uri &uri) const {
  pimpl_->get_uri(uri);
}

void request::get_uri(std::string &uri) const {
  pimpl_->get_uri(uri);
}

void request::get_version_major(unsigned short &major_version) {
  pimpl_->get_version_major(major_version);
}

void request::get_version_minor(unsigned short &minor_version) {
  pimpl_->get_version_minor(minor_version);
}

void request::get_method(std::string & method) const {
}

void request::get_status(std::string & status) const {
}

void request::get_status_message(std::string & status_message) const {
}

void request::get_body(std::function<void(char*, size_t)> chunk_reader) const {
}

void request::get_body(std::string const & body) const {
}

}  // namespace http

}  // namespace network

#endif /* NETWORK_PROTOCOL_HTTP_REQUEST_IPP_20110910 */
