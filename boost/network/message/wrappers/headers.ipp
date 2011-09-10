#ifndef BOOST_NETWORK_MESSAGE_WRAPPERS_HEADERS_IPP_20110911
#define BOOST_NETWORK_MESSAGE_WRAPPERS_HEADERS_IPP_20110911

// Copyright 2011 Dean Michael Berris <dberris@google.com>.
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/message/wrappers/headers.hpp>

namespace boost { namespace network {

headers_wrapper::headers_wrapper(message_base & message)
: message_(message)
{}

range_type headers_wrapper::operator[] (std::string const & key) const {
  cache_.reset(new (std::nothrow) container_type);
  if (!cache_.get())
    BOOST_THROW_EXCEPTION(std::runtime_error(
      "Cannot allocate cache multimap for headers wrapper."));
  message_.get_headers(
      key,
      std::inserter(*cache_, cache_->end()));
  return make_shared_container_range(cache_);
}

headers_wrapper::container_type::size_type
headers_wrapper::count(string_type const & key) const {
  this->init_cache_all();
  return cache_->size();
}

headers_wrapper::iterator headers_wrapper::begin() const {
  this->init_cache_all();
  return make_shared_container_iterator(cache_->begin());
}

headers_wrapper::iterator headers_wrapper::end() const {
  this->init_cache_all();
  return make_shared_container_iterator(cache_->end());
};

headers_wrapper::operator headers_wrapper::range_type () {
  this->init_cache_all();
  return make_shared_container_range(cache_);
};

headers_wrapper::operator headers_wrapper::container_type () {
  this->init_cache_all();
  return *cache_;
}

void headers_wrapper::init_cache_all() {
  if (!cache_.get()) {
    cache_.reset(new (std::nothrow) container_type);
    if (!cache_.get())
      BOOST_THROW_EXCEPTION(std::runtime_error(
          "Cannot allocate cache multimap for headers wrapper."));
    message_.get_headers(std::inserter(*cache_, cache_->end()));
  }
}

} /* network */

} /* boost */

#endif /* BOOST_NETWORK_MESSAGE_WRAPPERS_HEADERS_IPP_20110911 */
