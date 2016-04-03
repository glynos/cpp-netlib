//            Copyright (c) Glyn Matthews 2016.
//         (C) Copyright 2007-9 Anthony Williams
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_NETWORK_UTILS_THREAD_GROUP_INC
#define BOOST_NETWORK_UTILS_THREAD_GROUP_INC

#include <thread>
#include <mutex>
#include <vector>

namespace boost {
namespace network {
namespace utils {
class thread_group {
 public:
  thread_group() = default;
  ~thread_group() = default;
  thread_group(thread_group const&) = delete;
  thread_group& operator=(thread_group const&) = delete;

  template <typename F>
  std::thread &create_thread(F threadfunc) {
    std::lock_guard<std::mutex> guard(m);
    threads.emplace_back(threadfunc);
    return threads.back();
  }

  void join_all() {
    std::unique_lock<std::mutex> guard(m);
    for (auto& thread : threads) {
      if (thread.joinable()) {
        thread.join();
      }
    }
  }

  size_t size() const {
    std::unique_lock<std::mutex> guard(m);
    return threads.size();
  }

 private:
  std::vector<std::thread> threads;
  mutable std::mutex m;
};

}  // namespace utils
}  // namespace network
}  // namespace boost

#endif  // BOOST_NETWORK_UTILS_THREAD_GROUP_INC
