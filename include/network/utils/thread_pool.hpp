#ifndef BOOST_NETWORK_UTILS_THREAD_POOL_HPP_20101020
#define BOOST_NETWORK_UTILS_THREAD_POOL_HPP_20101020

// Copyright 2010 Dean Michael Berris. 
// Copyright 2012 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <cstddef>
#include <boost/thread/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/scope_exit.hpp>

namespace boost { namespace network { namespace utils {

typedef boost::shared_ptr<boost::asio::io_service> io_service_ptr;
typedef boost::shared_ptr<boost::thread_group> worker_threads_ptr;
typedef boost::shared_ptr<boost::asio::io_service::work> sentinel_ptr;

struct thread_pool_pimpl;

struct thread_pool {
  thread_pool(std::size_t threads = 1,
              io_service_ptr io_service = io_service_ptr(),
              worker_threads_ptr worker_threads = worker_threads_ptr());
  std::size_t const thread_count() const;
  void post(function<void()> f);
  ~thread_pool();
  void swap(thread_pool & other);
 protected:
  thread_pool_pimpl * pimpl;
};

inline void swap(thread_pool & l, thread_pool & r) {
  l.swap(r);
}

} // utils

} // network

} // boost
#endif /* BOOST_NETWORK_UTILS_THREAD_POOL_HPP_20101020 */
