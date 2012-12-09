// Copyright 2010 Dean Michael Berris.
// Copyright 2012 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef NETWORK_UTILS_THREAD_POOL_HPP_20101020
#define NETWORK_UTILS_THREAD_POOL_HPP_20101020

#include <cstddef>
#include <thread>
#include <memory>
#include <functional>
#include <vector>
#include <boost/asio/io_service.hpp>

namespace network { namespace utils {

  typedef std::shared_ptr<boost::asio::io_service> io_service_ptr;
  typedef std::shared_ptr<std::vector<std::thread>> worker_threads_ptr;
  typedef std::shared_ptr<boost::asio::io_service::work> sentinel_ptr;

  struct thread_pool_pimpl;

  struct thread_pool {
    thread_pool(std::size_t threads = 1,
                io_service_ptr io_service = io_service_ptr(),
                std::vector<std::thread> worker_threads = std::vector<std::thread>());
#if !defined(BOOST_NO_CXX11_DELETED_FUNCTIONS)
    thread_pool(thread_pool const&) = delete;
#endif // !defined(BOOST_NO_CXX11_DELETEED_FUNCTIONS)
    thread_pool(thread_pool &&other);
#if !defined(BOOST_NO_CXX11_DELETED_FUNCTIONS)
    thread_pool& operator=(thread_pool const&) = delete;
#endif // !defined(BOOST_NO_CXX11_DELETEED_FUNCTIONS)
    thread_pool& operator=(thread_pool &&other);
    std::size_t const thread_count() const;
    void post(std::function<void()> f);
    ~thread_pool();
    void swap(thread_pool & other);
  protected:
    thread_pool_pimpl * pimpl;
  };

  inline void swap(thread_pool & l, thread_pool & r) {
    l.swap(r);
  }

}  // namespace utils
}  // namespace network

#endif /* NETWORK_UTILS_THREAD_POOL_HPP_20101020 */
