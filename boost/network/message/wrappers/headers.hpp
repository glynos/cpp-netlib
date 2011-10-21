
//          Copyright Dean Michael Berris 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef __NETWORK_MESSAGE_WRAPPERS_HEADERS_HPP__
#define __NETWORK_MESSAGE_WRAPPERS_HEADERS_HPP__

#include <map>
#include <boost/shared_container_iterator.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/range/functions.hpp>
#include <boost/network/detail/wrapper_base.hpp>

namespace boost { namespace network {

namespace impl {

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
  typedef shared_container_iterator<container_type> iterator;
  typedef iterator_range<iterator> range_type;

  explicit headers_wrapper(message_base & message);
  range_type operator[] (std::string const & key) const;
  operator range_type () const;
  operator container_type () const;
  container_type::size_type count() const;
  container_type::size_type count(std::string const &key) const;
  iterator begin() const;
  iterator end() const;
 private:
  void init_cache_all() const;
  message_base & message_;
  mutable shared_ptr<container_type> cache_;
};

} // namespace impl

/// Factory method to create the right wrapper object
inline impl::headers_wrapper const
headers(message_base & message_) {
    return impl::headers_wrapper(message_);
}

} // namespace network

} // namespace boost

#ifdef BOOST_NETWORK_NO_LIB
#include <boost/network/message/wrappers/headers.ipp>
#endif

#endif // __NETWORK_MESSAGE_WRAPPERS_HEADERS_HPP__
