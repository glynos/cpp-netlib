// Copyright 2012 Dean Michael Berris <dberris@google.com>.
// Copyright 2012 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifdef BUILD_SHARED_LIBS
# define BOOST_TEST_DYN_LINK
#endif
#define BOOST_TEST_MODULE HTTP Request Storage Base Test
#include <network/protocol/http/request/request_base.hpp>
#include <boost/test/unit_test.hpp>

namespace http = network::http;

// In this test we make sure that the implementation of the default request
// storage base actually doesn't have bugs and works as advertised. Although we
// don't intend to expose this interface to users, we use the test as a sanity
// check on the internals of the implementation.
struct request_test : http::request_storage_base {
  typedef http::request_storage_base base_type;
  
  // Expose the protected functions so that we can test them.
  using base_type::append;
  using base_type::read;
  using base_type::flatten;
  using base_type::clear;

  explicit request_test(size_t chunk_size)
  : base_type(chunk_size)
  {}

  request_test(request_test const &other)
  : base_type(other)
  {}

  ~request_test() {
    // do nothing here.
  }
};

BOOST_AUTO_TEST_CASE(request_storage_flow) {
  // Use a few byte chunks just to make it manageable.
  request_test simple(64);
  static char data[] =
  "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Vivamus vitae ante sed nunc dapibus convallis in at neque. Vestibulum sed congue nunc. Sed tempus lorem non dui ultrices porttitor porta ligula venenatis. Sed a orci gravida tellus condimentum laoreet. Vivamus pulvinar, tortor eu adipiscing tempus, dolor urna tincidunt enim, id pretium eros ante quis dui. Class aptent taciti sociosqu ad litora torquent per conubia nostra, per inceptos himenaeos. In hac habitasse platea dictumst. Maecenas mattis metus.";
  simple.append(data, sizeof(data));
  char output[sizeof(data)];
  size_t bytes_read = simple.read(output, 0, sizeof(data));
  BOOST_CHECK_EQUAL(bytes_read, sizeof(data));
  std::string flattened;
  simple.flatten(flattened);
  BOOST_CHECK_EQUAL(flattened, std::string(output, sizeof(data)));
  BOOST_CHECK_EQUAL(std::string(data, sizeof(data)), std::string(output, sizeof(data)));
  simple.clear();
}

BOOST_AUTO_TEST_CASE(request_storage_copy) {
  // Use a few byt chunks just to make it manageable.
  request_test original(64);
  static char quick_brown[] = "The quick brown fox jumps over the lazy dog.";
  original.append(quick_brown, sizeof(quick_brown));
  char output[sizeof(quick_brown)];
  request_test copy(original);
  size_t bytes_read = copy.read(output, 0, sizeof(quick_brown));
  BOOST_CHECK_EQUAL(bytes_read, sizeof(quick_brown));
  std::string flattened;
  copy.flatten(flattened);
  BOOST_CHECK_EQUAL(flattened, std::string(output, sizeof(quick_brown)));
  BOOST_CHECK_EQUAL(std::string(quick_brown, sizeof(quick_brown)), std::string(output, sizeof(quick_brown)));
  copy.clear();
  flattened.clear();
  original.flatten(flattened);
  BOOST_CHECK_EQUAL(flattened, std::string(quick_brown, sizeof(quick_brown)));
}
