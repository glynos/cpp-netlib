// Copyright 2011 Dean Michael Berris <dberris@google.com>.
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef NETWORK_UTILS_THREAD_POOL_IPP_20111021
#define NETWORK_UTILS_THREAD_POOL_IPP_20111021

#include <vector>
#include <thread>
#include <network/utils/thread_pool.hpp>

namespace network { namespace utils {

  struct thread_pool_pimpl {
    thread_pool_pimpl(std::size_t threads = 1,
                      io_service_ptr io_service = io_service_ptr(),
                      std::vector<std::thread> worker_threads = {})
    : threads_(threads)
    , io_service_(io_service)
    , worker_threads_(std::move(worker_threads))
    , sentinel_()
    {
      bool commit = false;
      BOOST_SCOPE_EXIT((&commit)(&io_service_)(&worker_threads_)(&sentinel_)) {
        if (!commit) {
          sentinel_.reset();
          io_service_.reset();
          for (auto& thread : worker_threads_)
            if (thread.joinable()) thread.join();
          worker_threads_.clear();
        }
      } BOOST_SCOPE_EXIT_END

      if (!io_service_.get()) io_service_.reset(new boost::asio::io_service);
      if (!sentinel_.get())
        sentinel_.reset(new boost::asio::io_service::work(*io_service_));
      auto local_io_service = io_service_;
      for (std::size_t counter = 0; counter < threads_; ++counter)
        worker_threads_.emplace_back([local_io_service](){
          local_io_service->run();});

      commit = true;
    }

    thread_pool_pimpl(thread_pool_pimpl const &) = delete;
    thread_pool_pimpl & operator=(thread_pool_pimpl const &) = delete;

    thread_pool_pimpl(thread_pool_pimpl&& other) {
      other.swap(*this);
    }

    std::size_t const thread_count() const {
      return threads_;
    }

    void post(boost::function<void()> f) {
      io_service_->post(f);
    }

    ~thread_pool_pimpl() {
      sentinel_.reset();
      try {
        for (auto& thread : worker_threads_) thread.join();
      } catch (...) {
        BOOST_ASSERT(false && "A handler was not supposed to throw, but one did.");
        std::abort();
      }
    }

    void swap(thread_pool_pimpl & other) {
      using std::swap;
      swap(other.threads_, threads_);
      swap(other.io_service_, io_service_);
      swap(other.worker_threads_, worker_threads_);
      swap(other.sentinel_, sentinel_);
    }

  protected:
    std::size_t threads_;
    io_service_ptr io_service_;
    std::vector<std::thread> worker_threads_;
    sentinel_ptr sentinel_;
  };

  thread_pool::thread_pool(std::size_t threads,
                           io_service_ptr io_service,
                           std::vector<std::thread> worker_threads)
  : pimpl(new (std::nothrow) thread_pool_pimpl(threads, io_service, std::move(worker_threads)))
  {}

  std::size_t const thread_pool::thread_count() const {
    return pimpl->thread_count();
  }

  void thread_pool::post(boost::function<void()> f) {
    pimpl->post(f);
  }

  void thread_pool::swap(thread_pool & other) {
    std::swap(other.pimpl, this->pimpl);
  }

  thread_pool::~thread_pool() {
    delete pimpl;
  }

}  // namespace utils
}  // namespace network

#endif /* NETWORK_UTILS_THREAD_POOL_IPP_20111021 */
