#ifndef BOOST_NETWORK_MESSAGE_WRAPPERS_BODY_IPP_20111021
#define BOOST_NETWORK_MESSAGE_WRAPPERS_BODY_IPP_20111021

// Copyright 2011 Dean Michael Berris (dberris@google.com).
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/message/wrappers/body.hpp>

namespace boost { namespace network {

body_wrapper::body_wrapper(message_base const & message):
  message_(message) {}

body_wrapper::operator std::string () const {
  if (cache_) {
    return *cache_;
  }
  std::string tmp;
  message_.get_body(tmp);
  cache_ = tmp;
  return *cache_;
}

std::size_t body_wrapper::size() const {
  if (cache_) {
    return cache_->size();
  }
  std::string tmp;
  message_.get_body(tmp);
  cache_ = tmp;
  return cache_->size();
}

body_wrapper::operator boost::iterator_range<std::string::const_iterator> () const {
  if (cache_) {
    return boost::make_iterator_range(*cache_);
  }
  std::string tmp;
  message_.get_body(tmp);
  cache_ = tmp;
  return boost::make_iterator_range(*cache_);
}

std::string::const_iterator body_wrapper::begin() const {
  if (cache_) {
    return cache_->begin();
  }
  std::string tmp;
  message_.get_body(tmp);
  cache_ = tmp;
  return cache_->begin();
}

std::string::const_iterator body_wrapper::end() const {
  if (cache_) {
    return cache_->end();
  }
  std::string tmp;
  message_.get_body(tmp);
  cache_ = tmp;
  return cache_->end();
}

} /* network */
} /* boost */

#endif /* BOOST_NETWORK_MESSAGE_WRAPPERS_BODY_IPP_20111021 */
