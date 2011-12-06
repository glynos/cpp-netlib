#ifndef BOOST_NETWORK_PROTOCOL_HTTP_RESPONSE_RESPONSE_IPP_20111206
#define BOOST_NETWORK_PROTOCOL_HTTP_RESPONSE_RESPONSE_IPP_20111206

// Copyright 2011 Dean Michael Berris <dberris@google.com>.
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/protocol/http/response/response.hpp>
#include <boost/optional/optional.hpp>
#include <boost/utility/in_place_factory.hpp>

namespace boost { namespace network { namespace http {

struct response_pimpl {
  response_pimpl() {}
  response_pimpl * clone() {
    response_pimpl * new_pimpl = new (std::nothrow) response_pimpl;
    new_pimpl->source_future_ = source_future_;
    new_pimpl->destination_future_ = destination_future_;
    new_pimpl->headers_future_ = headers_future_;
    new_pimpl->status_future_ = status_future_;
    new_pimpl->status_message_future_ = status_message_future_;
    new_pimpl->version_future_ = version_future_;
    new_pimpl->body_future_ = body_future_;
    return new_pimpl;
  }

  void set_destination(std::string const &destination) {
    promise<std::string> destination_promise;
    unique_future<std::string> tmp_future = destination_promise.get_future();
    destination_future_ = move(tmp_future);
    destination_promise.set_value(destination);
  }

  void get_destination(std::string &destination) {
    if (!destination_future_) {
      destination = "";
    } else {
      destination = destination_future_->get();
    }
  }

  void set_source(std::string const &source) {
    promise<std::string> source_promise;
    unique_future<std::string> tmp_future = source_promise.get_future();
    source_future_ = move(tmp_future);
    source_promise.set_value(source);
  }

  void get_source(std::string &source) {
    if (!source_future_) {
      source = "";
    } else {
      source = source_future_->get();
    }
  }

  void append_header(std::string const & name,
                     std::string const & value) {
    // FIXME do something!
  }

  void remove_headers(std::string const &name) {
    // FIXME do something!
  }

  void remove_headers() {
    // FIXME do something!
  }

  void get_headers(
      function<void(std::string const &, std::string const &)> inserter) { /* FIXME: Do something! */ }
  void get_headers(
      std::string const & name,
      function<void(std::string const &, std::string const &)> inserter) { /* FIXME: Do something! */ }
  void get_headers(
      function<bool(std::string const &, std::string const &)> predicate,
      function<void(std::string const &, std::string const &)> inserter) { /* FIXME: Do something! */ }

  void set_body(std::string const &body) {
    promise<std::string> body_promise;
    unique_future<std::string> tmp_future = body_promise.get_future();
    body_future_ = move(tmp_future);
    body_promise.set_value(body);
  }

  void append_body(std::string const & data) { /* FIXME: Do something! */ }

  void get_body(std::string &body) {
    if (!body_future_) {
      body = "";
    } else {
      body = body_future_->get();
    }
  }

  void get_body(
      function<void(iterator_range<char const *>)> chunk_reader,
      size_t size) { /* FIXME: Do something! */ }

  void set_status(boost::uint16_t status) {
    promise<boost::uint16_t> status_promise;
    unique_future<boost::uint16_t> tmp_future = status_promise.get_future();
    status_future_ = move(tmp_future);
    status_promise.set_value(status);
  }

  void get_status(boost::uint16_t &status) {
    if (!status_future_) {
      status = 0u;
    } else {
      status = status_future_->get();
    }
  }

  void set_status_message(std::string const &status_message) {
    promise<std::string> status_message_promise_;
    unique_future<std::string> tmp_future = status_message_promise_.get_future();
    status_message_future_ = move(tmp_future);
    status_message_promise_.set_value(status_message);
  }

  void get_status_message(std::string &status_message) {
    if (!status_message_future_) {
      status_message = "";
    } else {
      status_message = status_message_future_->get();
    }
  }

  void set_version(std::string const &version) {
    promise<std::string> version_promise;
    unique_future<std::string> tmp_future = version_promise.get_future();
    version_future_ = move(tmp_future);
    version_promise.set_value(version);
  }

  void get_version(std::string &version) {
    if (!version_future_) {
      version = "";
    } else {
      version = version_future_->get();
    }
  }

  promise<std::string> get_source_promise() {
    promise<std::string> promise_;
    unique_future<std::string> tmp_future = promise_.get_future();
    source_future_ = move(tmp_future);
    return move(promise_);
  }

  promise<std::string> get_destination_promise() {
    promise<std::string> promise_;
    unique_future<std::string> tmp_future = promise_.get_future();
    destination_future_ = move(tmp_future);
    return move(promise_);
  }

  promise<std::multimap<std::string, std::string> > get_headers_promise() {
    promise<std::multimap<std::string, std::string> > promise_;
    unique_future<std::multimap<std::string, std::string> > tmp_future = promise_.get_future();
    headers_future_ = move(tmp_future);
    return promise_;
  }

  promise<boost::uint16_t> get_status_promise() {
    promise<boost::uint16_t> promise_;
    unique_future<boost::uint16_t> tmp_future = promise_.get_future();
    status_future_ = move(tmp_future);
    return promise_;
  }

  promise<std::string> get_status_message_promise() {
    promise<std::string> promise_;
    unique_future<std::string> tmp_future = promise_.get_future();
    status_message_future_ = move(tmp_future);
    return promise_;
  }

  promise<std::string> get_version_promise() {
    promise<std::string> promise_;
    unique_future<std::string> tmp_future = promise_.get_future();
    version_future_ = move(tmp_future);
    return promise_;
  }

  promise<std::string> get_body_promise() {
    promise<std::string> promise_;
    unique_future<std::string> tmp_future = promise_.get_future();
    body_future_ = move(tmp_future);
    return promise_;
  }

 private:
  optional<shared_future<std::string> > source_future_;
  optional<shared_future<std::string> > destination_future_;
  optional<shared_future<std::multimap<std::string, std::string> > >
      headers_future_;
  optional<shared_future<boost::uint16_t> > status_future_;
  optional<shared_future<std::string> > status_message_future_;
  optional<shared_future<std::string> > version_future_;
  optional<shared_future<std::string> > body_future_;
};

response::response()
: pimpl_(new (std::nothrow) response_pimpl)
{}

response::response(response const & other)
: pimpl_(other.pimpl_->clone())
{}

response& response::operator=(response rhs) {
  rhs.swap(*this);
  return *this;
}

void response::swap(response &other) {
  other.pimpl_.swap(pimpl_);
}

void response::set_destination(std::string const &destination) {
  pimpl_->set_destination(destination);
}

void response::set_source(std::string const &source) {
  pimpl_->set_source(source);
}

void response::append_header(std::string const &name,
                             std::string const &value) {
  pimpl_->append_header(name, value);
}

void response::remove_headers(std::string const &name) {
  pimpl_->remove_headers(name);
}

void response::remove_headers() {
  pimpl_->remove_headers();
}

void response::set_body(std::string const &body) {
  pimpl_->set_body(body);
}

void response::append_body(std::string const &data) {
  pimpl_->append_body(data);
}

void response::get_destination(std::string &destination) const {
  pimpl_->get_destination(destination);
}

void response::get_source(std::string &source) const {
  pimpl_->get_source(source);
}

void response::get_headers(function<void(std::string const &, std::string const &)> inserter) const {
  pimpl_->get_headers(inserter);
}

void response::get_headers(std::string const &name,
                           function<void(std::string const &, std::string const &)> inserter) const {
  pimpl_->get_headers(name, inserter);
}

void response::get_headers(function<bool(std::string const &, std::string const &)> predicate,
                           function<void(std::string const &, std::string const &)> inserter) const {
  pimpl_->get_headers(predicate, inserter);
}

void response::get_body(std::string &body) const {
  pimpl_->get_body(body);
}

void response::get_body(function<void(iterator_range<char const *>)> chunk_reader, size_t size) const {
  pimpl_->get_body(chunk_reader, size);
}

void response::set_status(boost::uint16_t new_status) {
  pimpl_->set_status(new_status);
}

void response::set_status_message(std::string const &new_status_message) {
  pimpl_->set_status_message(new_status_message);
}

void response::set_version(std::string const &new_version) {
  pimpl_->set_version(new_version);
}

void response::get_status(boost::uint16_t &status) const {
  pimpl_->get_status(status);
}

void response::get_status_message(std::string &status_message) const {
  pimpl_->get_status_message(status_message);
}

void response::get_version(std::string &version) const {
  pimpl_->get_version(version);
}

response::~response() {}

promise<std::string> response::get_version_promise() {
  return pimpl_->get_version_promise();
}

promise<boost::uint16_t> response::get_status_promise() {
  return pimpl_->get_status_promise();
}

promise<std::string> response::get_status_message_promise() {
  return pimpl_->get_status_message_promise();
}

promise<std::multimap<std::string, std::string> > response::get_headers_promise() {
  return pimpl_->get_headers_promise();
}

promise<std::string> response::get_source_promise() {
  return pimpl_->get_source_promise();
}

promise<std::string> response::get_destination_promise() {
  return pimpl_->get_destination_promise();
}

promise<std::string> response::get_body_promise() {
  return pimpl_->get_body_promise();
}

}  // namespace http

}  // namespace network

}  // namespace boost

#endif  // BOOST_NETWORK_PROTOCOL_HTTP_RESPONSE_RESPONSE_IPP_20111206
