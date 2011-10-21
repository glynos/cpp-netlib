#ifndef BOOST_NETWORK_MESSAGE_WRAPPERS_HEADERS_IPP_20110911
#define BOOST_NETWORK_MESSAGE_WRAPPERS_HEADERS_IPP_20110911

// Copyright 2011 Dean Michael Berris <dberris@google.com>.
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/message/wrappers/headers.hpp>
#include <boost/function.hpp>

namespace boost { namespace network { namespace impl {

headers_wrapper::headers_wrapper(message_base & message)
: message_(message)
{}

headers_wrapper::range_type headers_wrapper::operator[] (std::string const & key) const {
  this->init_cache_all();
  std::pair<container_type::iterator, container_type::iterator> p =
      cache_->equal_range(key);
  return boost::make_iterator_range(
      boost::make_shared_container_iterator(
          p.first, cache_),
      boost::make_shared_container_iterator(
          p.second, cache_));
}

headers_wrapper::container_type::size_type
headers_wrapper::count(std::string const & key) const {
  this->init_cache_all();
  return cache_->size();
}

headers_wrapper::iterator headers_wrapper::begin() const {
  this->init_cache_all();
  container_type::iterator begin = cache_->begin();
  return make_shared_container_iterator(begin, cache_);
}

headers_wrapper::iterator headers_wrapper::end() const {
  this->init_cache_all();
  container_type::iterator end = cache_->end();
  return make_shared_container_iterator(end, cache_);
};

headers_wrapper::operator headers_wrapper::range_type () const {
  this->init_cache_all();
  return make_shared_container_range(cache_);
};

headers_wrapper::operator headers_wrapper::container_type () const {
  this->init_cache_all();
  return *cache_;
}

template <class Map>
struct kv_inserter {
  kv_inserter(Map & m)
      : m_(m) {}
  void operator() (std::string const & k, std::string const & v) const {
    m_.insert(std::make_pair(k, v));
  }
 private:
  Map & m_;
};

void headers_wrapper::init_cache_all() const {
  if (!cache_.get()) {
    cache_.reset(new (std::nothrow) container_type);
    if (!cache_.get())
      BOOST_THROW_EXCEPTION(std::runtime_error(
          "Cannot allocate cache multimap for headers wrapper."));
    message_.get_headers(kv_inserter<container_type>(*cache_));
  }
}

} /* impl */

} /* network */

} /* boost */

#endif /* BOOST_NETWORK_MESSAGE_WRAPPERS_HEADERS_IPP_20110911 */
