// Copyright 2011 Dean Michael Berris <dberris@google.com>.
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef NETWORK_UTILS_THREAD_POOL_IPP_20111021
#define NETWORK_UTILS_THREAD_POOL_IPP_20111021

#include <network/utils/thread_pool.hpp>

namespace network { namespace utils {

struct thread_pool_pimpl {
    thread_pool_pimpl(
        std::size_t threads = 1, 
        io_service_ptr io_service = io_service_ptr(), 
        worker_threads_ptr worker_threads = worker_threads_ptr()
        ) 
    : threads_(threads)
    , io_service_(io_service)
    , worker_threads_(worker_threads)
    , sentinel_()
    {
        bool commit = false;
        BOOST_SCOPE_EXIT((&commit)(&io_service_)(&worker_threads_)(&sentinel_)) {
            if (!commit) {
                sentinel_.reset();
                io_service_.reset();
                if (worker_threads_.get()) {
                    worker_threads_->interrupt_all();
                    worker_threads_->join_all();
                }
                worker_threads_.reset();
            }
        } BOOST_SCOPE_EXIT_END

        if (!io_service_.get()) {
            io_service_.reset(new asio::io_service);
        }

        if (!worker_threads_.get()) {
            worker_threads_.reset(new boost::thread_group);
        }

        if (!sentinel_.get()) {
            sentinel_.reset(new asio::io_service::work(*io_service_));
        }

        auto io_service_local = io_service_;
        for (std::size_t counter = 0; counter < threads_; ++counter)
            worker_threads_->emplace_back([=io_service_local]() { io_service_local->run() });

        commit = true;
    }

    std::size_t const thread_count() const {
        return threads_;
    }

    void post(std::function<void()> f) {
        io_service_->post(f);
    }

    ~thread_pool_pimpl() {
        sentinel_.reset();
        try {
            worker_threads_->join_all();
        } catch (...) {
            BOOST_ASSERT(false && "A handler was not supposed to throw, but one did.");
            std::abort();
        }
    }

    void swap(thread_pool_pimpl & other) {
        std::swap(other.threads_, threads_);
        std::swap(other.io_service_, io_service_);
        std::swap(other.worker_threads_, worker_threads_);
        std::swap(other.sentinel_, sentinel_);
    }
protected:
    std::size_t threads_;
    io_service_ptr io_service_;
    worker_threads_ptr worker_threads_;
    sentinel_ptr sentinel_;

private:
    thread_pool_pimpl(thread_pool_pimpl const &); // no copies please
    thread_pool_pimpl & operator=(thread_pool_pimpl); // no assignment please
};

thread_pool::thread_pool(std::size_t threads,
                         io_service_ptr io_service,
                         worker_threads_ptr worker_threads)
: pimpl(new (std::nothrow) thread_pool_pimpl(threads, io_service, worker_threads))
{}

std::size_t const thread_pool::thread_count() const {
  return pimpl->thread_count();
}

void thread_pool::post(std::function<void()> f) {
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
