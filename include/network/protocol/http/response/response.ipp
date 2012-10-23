// Copyright 2011 Dean Michael Berris <dberris@google.com>.
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef NETWORK_PROTOCOL_HTTP_RESPONSE_RESPONSE_IPP_20111206
#define NETWORK_PROTOCOL_HTTP_RESPONSE_RESPONSE_IPP_20111206

#include <network/protocol/http/response/response.hpp>
#include <set>

namespace network { namespace http {

struct response_pimpl {
  response_pimpl() {}

  response_pimpl * clone() {
    return new (std::nothrow) response_pimpl(*this);
  }

  void set_destination(std::string const &destination) {
    std::promise<std::string> destination_promise;
    destination_promise.set_value(destination);
    std::future<std::string> tmp_future = destination_promise.get_future();
    destination_future_ = std::move(tmp_future);
  }

  void get_destination(std::string &destination) {
    if (!destination_future_.valid()) {
      destination = "";
    } else {
      destination = destination_future_.get();
    }
  }

  void set_source(std::string const &source) {
    std::promise<std::string> source_promise;
    source_promise.set_value(source);
    source_future_ = source_promise.get_future().share();
  }

  void get_source(std::string &source) {
    if (!source_future_.valid()) {
      source = "";
    } else {
      source = source_future_.get();
    }
  }

  void append_header(std::string const & name,
                     std::string const & value) {
    added_headers_.insert(std::make_pair(name, value));
  }

  void remove_headers(std::string const &name) {
    removed_headers_.insert(name);
  }

  void remove_headers() {
    if (!headers_future_.valid()) {
      std::promise<std::multimap<std::string, std::string> > headers_promise;
      headers_promise.set_value(std::multimap<std::string, std::string>());
      std::future<std::multimap<std::string, std::string> > tmp =
          headers_promise.get_future();
      std::multimap<std::string, std::string>().swap(added_headers_);
      std::set<std::string>().swap(removed_headers_);
      headers_future_ = std::move(tmp);
    }
  }

  void get_headers(
      std::function<void(std::string const &, std::string const &)> inserter) {
    std::multimap<std::string, std::string>::const_iterator it;
    if (!headers_future_.valid()) {
      it = added_headers_.begin();
      for (;it != added_headers_.end(); ++it) {
        if (removed_headers_.find(it->first) == removed_headers_.end()) {
          inserter(it->first, it->second);
        }
      }
    } else {
      std::multimap<std::string, std::string> const & headers_ =
          headers_future_.get();
      it = headers_.begin();
      for (;it != headers_.end(); ++it) {
        if (removed_headers_.find(it->first) == removed_headers_.end()) {
          inserter(it->first, it->second);
        }
      }
    }
  }
  void get_headers(
      std::string const & name,
      std::function<void(std::string const &, std::string const &)> inserter) { /* FIXME: Do something! */ }
  void get_headers(
      std::function<bool(std::string const &, std::string const &)> predicate,
      std::function<void(std::string const &, std::string const &)> inserter) { /* FIXME: Do something! */ }

  void set_body(std::string const &body) {
    std::promise<std::string> body_promise;
    body_promise.set_value(body);
    std::future<std::string> tmp_future = body_promise.get_future();
    body_future_ = std::move(tmp_future);
  }

  void append_body(std::string const & data) { /* FIXME: Do something! */ }

  void get_body(std::string &body) {
    if (!body_future_.valid()) {
      body = "";
    } else {
      body = body_future_.get();
    }
  }

  void get_body(
      std::function<void(boost::iterator_range<char const *>)> chunk_reader,
      size_t size) { /* FIXME: Do something! */ }

  void set_status(boost::uint16_t status) {
    std::promise<boost::uint16_t> status_promise;
    status_promise.set_value(status);
    std::future<boost::uint16_t> tmp_future = status_promise.get_future();
    status_future_ = std::move(tmp_future);
  }

  void get_status(boost::uint16_t &status) {
    if (!status_future_.valid()) {
      status = 0u;
    } else {
      status = status_future_.get();
    }
  }

  void set_status_message(std::string const &status_message) {
    std::promise<std::string> status_message_promise_;
    status_message_promise_.set_value(status_message);
    std::future<std::string> tmp_future = status_message_promise_.get_future();
    status_message_future_ = std::move(tmp_future);
  }

  void get_status_message(std::string &status_message) {
    if (!status_message_future_.valid()) {
      status_message = "";
    } else {
      status_message = status_message_future_.get();
    }
  }

  void set_version(std::string const &version) {
    std::promise<std::string> version_promise;
    version_promise.set_value(version);
    std::future<std::string> tmp_future = version_promise.get_future();
    version_future_ = std::move(tmp_future);
  }

  void get_version(std::string &version) {
    if (!version_future_.valid()) {
      version = "";
    } else {
      version = version_future_.get();
    }
  }

  void set_source_promise(std::promise<std::string> &promise_) {
    std::future<std::string> tmp_future = promise_.get_future();
    source_future_ = std::move(tmp_future);
  }

  void set_destination_promise(std::promise<std::string> &promise_) {
    std::future<std::string> tmp_future = promise_.get_future();
    destination_future_ = std::move(tmp_future);
  }

  void set_headers_promise(std::promise<std::multimap<std::string, std::string> > &promise_) {
    std::future<std::multimap<std::string, std::string> > tmp_future = promise_.get_future();
    headers_future_ = std::move(tmp_future);
  }

  void set_status_promise(std::promise<boost::uint16_t> &promise_) {
    std::future<boost::uint16_t> tmp_future = promise_.get_future();
    status_future_ = std::move(tmp_future);
  }

  void set_status_message_promise(std::promise<std::string> &promise_) {
    std::future<std::string> tmp_future = promise_.get_future();
    status_message_future_ = std::move(tmp_future);
  }

  void set_version_promise(std::promise<std::string> &promise_) {
    std::future<std::string> tmp_future = promise_.get_future();
    version_future_ = std::move(tmp_future);
  }

  void set_body_promise(std::promise<std::string> &promise_) {
    std::future<std::string> tmp_future = promise_.get_future();
    body_future_ = std::move(tmp_future);
  }

  bool equals(response_pimpl const &other) {
    if (source_future_.valid()) {
      if (!other.source_future_.valid())
        return false;
      if (source_future_.get() != other.source_future_.get())
        return false;
    } else {
      if (other.source_future_.valid())
        return false;
    }
    if (destination_future_.valid()) {
      if (!other.destination_future_.valid())
        return false;
      if (destination_future_.get() != other.destination_future_.get())
        return false;
    } else {
      if (other.destination_future_.valid())
        return false;
    }
    if (headers_future_.valid()) {
      if (!other.headers_future_.valid())
        return false;
      if (headers_future_.get() != other.headers_future_.get())
        return false;
    } else {
      if (other.headers_future_.valid())
        return false;
    }
    if (status_future_.valid()) {
      if (!other.status_future_.valid())
        return false;
      if (status_future_.get() != other.status_future_.get())
        return false;
    } else {
      if (other.status_future_.valid())
        return false;
    }
    if (status_message_future_.valid()) {
      if (!other.status_message_future_.valid())
        return false;
      if (status_message_future_.get() != other.status_message_future_.get())
        return false;
    } else {
      if (other.status_message_future_.valid())
        return false;
    }
    if (version_future_.valid()) {
      if (!other.version_future_.valid())
        return false;
      if (version_future_.get() != other.version_future_.get())
        return false;
    } else {
      if (other.version_future_.valid())
        return false;
    }
    if (body_future_.valid()) {
      if (!other.body_future_.valid())
        return false;
      if (body_future_.get() != other.body_future_.get())
        return false;
    } else {
      if (other.body_future_.valid())
        return false;
    }
    if (other.added_headers_ != added_headers_ || other.removed_headers_ != removed_headers_)
      return false;
    return true;
  }

 private:
  mutable std::shared_future<std::string> source_future_;
  mutable std::shared_future<std::string> destination_future_;
  mutable std::shared_future<std::multimap<std::string, std::string> >
      headers_future_;
  mutable std::shared_future<boost::uint16_t> status_future_;
  mutable std::shared_future<std::string> status_message_future_;
  mutable std::shared_future<std::string> version_future_;
  mutable std::shared_future<std::string> body_future_;
  // TODO: use unordered_map and unordered_set here.
  std::multimap<std::string, std::string> added_headers_;
  std::set<std::string> removed_headers_;

  response_pimpl(response_pimpl const &other)
  : source_future_(other.source_future_)
  , destination_future_(other.destination_future_)
  , headers_future_(other.headers_future_)
  , status_future_(other.status_future_)
  , status_message_future_(other.status_message_future_)
  , version_future_(other.version_future_)
  , body_future_(other.body_future_)
  , added_headers_(other.added_headers_)
  , removed_headers_(other.removed_headers_)
  {}
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
  std::swap(this->pimpl_, other.pimpl_);
}

bool response::equals(response const &other) const {
  return other.pimpl_->equals(*pimpl_);
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

void response::get_headers(std::function<void(std::string const &, std::string const &)> inserter) const {
  pimpl_->get_headers(inserter);
}

void response::get_headers(std::string const &name,
                           std::function<void(std::string const &, std::string const &)> inserter) const {
  pimpl_->get_headers(name, inserter);
}

void response::get_headers(std::function<bool(std::string const &, std::string const &)> predicate,
                           std::function<void(std::string const &, std::string const &)> inserter) const {
  pimpl_->get_headers(predicate, inserter);
}

void response::get_body(std::string &body) const {
  pimpl_->get_body(body);
}

void response::get_body(std::function<void(boost::iterator_range<char const *>)> chunk_reader, size_t size) const {
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

response::~response() {
  delete pimpl_;
}

void response::set_version_promise(std::promise<std::string> &promise) {
  return pimpl_->set_version_promise(promise);
}

void response::set_status_promise(std::promise<boost::uint16_t> &promise) {
  return pimpl_->set_status_promise(promise);
}

void response::set_status_message_promise(std::promise<std::string> &promise) {
  return pimpl_->set_status_message_promise(promise);
}

void response::set_headers_promise(std::promise<std::multimap<std::string, std::string> > &promise) {
  return pimpl_->set_headers_promise(promise);
}

void response::set_source_promise(std::promise<std::string> &promise) {
  return pimpl_->set_source_promise(promise);
}

void response::set_destination_promise(std::promise<std::string> &promise) {
  return pimpl_->set_destination_promise(promise);
}

void response::set_body_promise(std::promise<std::string> &promise) {
  return pimpl_->set_body_promise(promise);
}

}  // namespace http

}  // namespace network

#endif  // NETWORK_PROTOCOL_HTTP_RESPONSE_RESPONSE_IPP_20111206
