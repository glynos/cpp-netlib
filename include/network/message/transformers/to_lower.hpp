
//          Copyright Dean Michael Berris 2007.
// Copyright 2012 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef __NETWORK_MESSAGE_TRANSFORMERS_TO_LOWER_HPP__
#define __NETWORK_MESSAGE_TRANSFORMERS_TO_LOWER_HPP__

#include <boost/algorithm/string.hpp>
#include <network/message/message_base.hpp>

/** to_lower.hpp
 *
 * Implements the to_lower transformer. This applies
 * the to_lower string algorithm to a string, which
 * is selected by the appropriate selector.
 *
 * This defines a type, to be applied using template
 * metaprogramming on the selected string target.
 */
namespace boost { namespace network { namespace impl {

template <class Selector>
struct to_lower_transformer { };

template <>
struct to_lower_transformer<selectors::source_selector> {
  void operator() (message_base & message_) const {
    std::string source_;
    message_.get_source(source_);
    boost::to_lower(source_);
    message_.set_source(source_);
  }

 protected:
  ~to_lower_transformer() { }
};

template <>
struct to_lower_transformer<selectors::destination_selector> {
  void operator() (message_base & message_) const {
    std::string destination_;
    message_.get_destination(destination_);
    boost::to_lower(destination_);
    message_.set_destination(destination_);
  }

 protected:
  ~to_lower_transformer() { };
};

} // namespace impl

namespace detail {
  struct to_lower_placeholder_helper;
}

detail::to_lower_placeholder_helper to_lower_(detail::to_lower_placeholder_helper);

namespace detail {

struct to_lower_placeholder_helper {
  template <class Selector>
      struct type : public impl::to_lower_transformer<Selector> { };
 private:
  to_lower_placeholder_helper() {}
  to_lower_placeholder_helper(to_lower_placeholder_helper const &) {}
  friend to_lower_placeholder_helper boost::network::to_lower_(to_lower_placeholder_helper);
};

}

typedef detail::to_lower_placeholder_helper (*to_lower_placeholder)(detail::to_lower_placeholder_helper);

inline detail::to_lower_placeholder_helper to_lower_(detail::to_lower_placeholder_helper) {
  return detail::to_lower_placeholder_helper();
}

} // namespace network

} // namespace boost

#endif // __NETWORK_MESSAGE_TRANSFORMERS_TO_LOWER_HPP__

