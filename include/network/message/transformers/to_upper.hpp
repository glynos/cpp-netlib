
// Copyright Dean Michael Berris 2007.
// Copyright 2012 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef NETWORK_MESSAGE_TRANSFORMERS_TO_UPPER_HPP
#define NETWORK_MESSAGE_TRANSFORMERS_TO_UPPER_HPP

#include <boost/algorithm/string.hpp>
#include <network/message/message_base.hpp>

/** to_upper.hpp
 *
 * Implements the to_upper transformer. This applies
 * the to_upper string algorithm to a string, which
 * is selected by the appropriate selector.
 *
 * This defines a type, to be applied using template
 * metaprogramming on the selected string target.
 */
namespace network {
namespace impl {

template <class Selector>
struct to_upper_transformer {};

template <>
struct to_upper_transformer<selectors::source_selector> {
  void operator() (message_base & message_) const {
    std::string source_;
    message_.get_source(source_);
    boost::to_upper(source_);
    message_.set_source(source_);
  }

 protected:
  ~to_upper_transformer() {};
};

template <>
struct to_upper_transformer<selectors::destination_selector> {
  void operator() (message_base & message_) const {
    std::string destination_;
    message_.get_destination(destination_);
    boost::to_upper(destination_);
    message_.set_destination(destination_);
  }

 protected:
  ~to_upper_transformer() {};
};

} // namespace impl

namespace detail {
  struct to_upper_placeholder_helper;
}  // namespace detail

detail::to_upper_placeholder_helper to_upper_(detail::to_upper_placeholder_helper);

namespace detail {

struct to_upper_placeholder_helper {
  template <class Selector>
  struct type : public impl::to_upper_transformer<Selector> { };

 private:
  to_upper_placeholder_helper() {}
  to_upper_placeholder_helper(to_upper_placeholder_helper const &) {}
  friend to_upper_placeholder_helper network::to_upper_(to_upper_placeholder_helper);
};

}  // namespace detail

typedef detail::to_upper_placeholder_helper (*to_upper_placeholder)(detail::to_upper_placeholder_helper);

inline detail::to_upper_placeholder_helper to_upper_(detail::to_upper_placeholder_helper) {
    return detail::to_upper_placeholder_helper();
}

} // namespace network

#endif // NETWORK_MESSAGE_TRANSFORMERS_TO_UPPER_HPP
