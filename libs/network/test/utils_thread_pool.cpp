
// Copyright 2010 Dean Michael Berris.
// Copyright 2015 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>
#include <atomic>
#include <boost/network/utils/thread_pool.hpp>

using namespace boost::network;

// This test specifies the requirements for a thread pool interface. At the
// very least any thread pool implementation should be able to pass the simple
// tests that this unit test requires of thread pools. Ultimately the
// requirements will show up in the Concept documentation, but this test is the
// canonical definition of what a thread pool should look like at least
// syntactically.
//

TEST(ThreadPoolTest, DefaultConstructor) {
  utils::thread_pool pool;
  ASSERT_EQ(1, pool.thread_count());
}

struct foo {
  foo() : val_(0) {}
  void bar(int val) { val_ += val; }
  int val() const { return val_; }

 protected:
  int val_;
};

TEST(ThreadPoolTest, PostWork) {
  foo instance;
  {
    utils::thread_pool pool;
    ASSERT_NO_THROW(pool.post([&instance] { instance.bar(1); }));
    ASSERT_NO_THROW(pool.post([&instance] { instance.bar(2); }));
    // require that pool is destroyed here, RAII baby
  }
  ASSERT_EQ(instance.val(), 3);
}

// Test using multiple threads.
TEST(ThreadPoolTest, MultipleThreads) {
  std::atomic<int64_t> counter{0};
  constexpr int64_t kMaxCount = 1e5;
  {
    utils::thread_pool pool(8);  // nice round number of threads.
    for (int64_t i = 0; i < kMaxCount; ++i) {
      ASSERT_NO_THROW(pool.post(
          [&counter] { counter.fetch_add(1, std::memory_order_acq_rel); }));
    }
    // Wait for threads to be done.
  }
  ASSERT_EQ(kMaxCount, counter.load(std::memory_order_acquire));
}
