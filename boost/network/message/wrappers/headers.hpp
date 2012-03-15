
//          Copyright Dean Michael Berris 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef NETWORK_MESSAGE_WRAPPERS_HEADERS_HPP__
#define NETWORK_MESSAGE_WRAPPERS_HEADERS_HPP__

#include <map>

namespace boost { namespace network {

struct message_base;

/** headers wrapper for messages.
 *
 * This exposes an interface similar to a map, indexable
 * using operator[] taking a string as the index and returns
 * a range of iterators (std::pair<iterator, iterator>)
 * whose keys are all equal to the index string.
 *
 */
struct headers_wrapper {
  typedef std::multimap<std::string, std::string> container_type;
  explicit headers_wrapper(message_base const & message);
  operator container_type () const;
 private:
  message_base const & message_;
};

/// Factory method to create the right wrapper object
inline headers_wrapper const
headers(message_base const & message_) {
    return headers_wrapper(message_);
}

} // namespace network

} // namespace boost

#endif // __NETWORK_MESSAGE_WRAPPERS_HEADERS_HPP__
