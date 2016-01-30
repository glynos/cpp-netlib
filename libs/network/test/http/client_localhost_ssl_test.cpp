
// Copyright Divye Kapoor 2008.
// Copyright 2016 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt)
//
// Changes by Kim Grasman 2008
// Changes by Dean Michael Berris 2008, 2009, 2016

#include <gtest/gtest.h>
#include <boost/config/warning_disable.hpp>
#include <boost/config.hpp>
#include <boost/network/protocol/http/client.hpp>
#include <boost/cast.hpp>
#include <string>
#include <fstream>
#include <iostream>

#define HTTPS_SERVER_TEST
#include "http_test_server.hpp"
#include "client_types.hpp"

using std::cout;
using std::endl;

namespace {
const std::string base_url = "https://localhost:8443";
const std::string cgi_url = base_url + "/cgi-bin/requestinfo.py";

class RunningServerEnvironment : public ::testing::Environment {
 public:
  void SetUp() override {
    if (!server.start())
      cout << "Failed to start HTTP server for test!" << endl;
  }

  void TearDown() override {
    if (!server.stop()) cout << "Failed to stop HTTP server for test!" << endl;
  }

  http_test_server server;
};

std::size_t readfile(std::ifstream& file, std::vector<char>& buffer) {
  using std::ios;

  std::istreambuf_iterator<char> src(file);
  std::istreambuf_iterator<char> eof;
  std::copy(src, eof, std::back_inserter(buffer));

  return buffer.size();
}
}

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
// Uncomment the below if you're running Python pre-2.6. There was a bug
// in the Python HTTP server for earlier versions that causes this test
// case to fail.
// BOOST_AUTO_TEST_CASE_EXPECTED_FAILURES(text_query_preserves_crlf, 2);
#endif

auto *local_env = ::testing::AddGlobalTestEnvironment(new RunningServerEnvironment());

TYPED_TEST_CASE(HTTPClientTest, ClientTypes);

TYPED_TEST(HTTPClientTest, BodyTest) {
  // Tests presence of body in http responses
  using namespace boost::network;
  using client = TypeParam;
  typename client::request request_(base_url);
  client client_;
  typename client::response response_ = client_.get(request_);
  EXPECT_NE(0, body(response_).size());
}

TYPED_TEST(HTTPClientTest, TextContentTypeTest) {
  // Tests correct parsing of the content-type header sent by the server
  using namespace boost::network;
  using client = TypeParam;
  typename client::request request_(base_url);
  client client_;
  typename client::response response_ = client_.get(request_);
  ASSERT_NE(0, headers(response_).count("Content-type"));
  auto range = headers(response_)["Content-type"];
  EXPECT_EQ("Content-type", std::begin(range)->first);
  EXPECT_EQ("text/html", std::begin(range)->second);
}

TYPED_TEST(HTTPClientTest, BinaryContentTypeTest) {
  // Tests correct parsing of content-type for binary files such as .zip files
  using namespace boost::network;
  using client = TypeParam;
  typename client::request request_(base_url + "/boost.jpg");
  client client_;
  typename client::response response_ = client_.get(request_);
  BOOST_REQUIRE(headers(response_).count("Content-type") != 0);
  auto range = headers(response_)["Content-type"];
  EXPECT_EQ("Content-type", std::begin(range)->first);
  EXPECT_EQ("image/jpeg", std::begin(range)->second);
}

TYPED_TEST(HTTPClientTest, ContentLengthHeaderTest) {
  // Uses the test.xml file to ensure that the file was received at the correct
  // length for a text encoding
  using namespace boost::network;
  using client = TypeParam;
  typename client::request request_(base_url + "/test.xml");
  client client_;
  typename client::response response_ = client_.get(request_);
  BOOST_REQUIRE(headers(response_).count("Content-Length") != 0);
  auto range = headers(response_)["Content-Length"];
  EXPECT_EQ("Content-Length", std::begin(range)->first);
  EXPECT_EQ("113", std::begin(range)->second);
  EXPECT_NE(0, body(response_).size());
}

TYPED_TEST(HTTPClientTest, TextQueryPreservesCRLFTest) {
  // Tests proper transfer of a text file
  using namespace boost::network;
  using client = TypeParam;
  typename client::request request_(base_url + "/test.xml");
  client client_;
  typename client::response response_ = client_.get(request_);

  http::client::response::string_type body_ = body(response_);
  EXPECT_NE(0, body_.size());

  using std::ios;

  std::ifstream file("libs/network/test/server/test.xml",
                     ios::in | ios::binary);
  if (!file) {
    file.clear();
    file.open("server/test.xml", ios::in | ios::binary);
  }

  ASSERT_TRUE(file) << "Could not open local test.xml";

  std::vector<char> memblock;
  std::size_t size = readfile(file, memblock);

  EXPECT_NE(0, size);
  EXPECT_EQ(size, body(response_).size());

  if (body(response_).size() == size) {
    auto diff_pos =
        std::mismatch(memblock.begin(), memblock.end(), body_.begin());
    EXPECT_EQ(
        boost::numeric_cast<std::size_t>(diff_pos.first - memblock.begin()),
        size);
  }
}

TYPED_TEST(HTTPClientTest, BinaryFileQueryTest) {
  // Tests proper transfer of a binary image
  using namespace boost::network;
  using client = TypeParam;
  typename client::request request_(base_url + "/boost.jpg");
  client client_;
  typename client::response response_;
  ASSERT_NO_THROW(response_ = client_.get(request_));
  typename client::response::string_type body_ = body(response_);
  EXPECT_NE(0, body_.size());

  using std::ios;

  std::ifstream file("libs/network/test/server/boost.jpg",
                     ios::in | ios::binary);
  if (!file) {
    file.clear();
    file.open("server/boost.jpg", ios::in | ios::binary);
  }

  ASSERT_TRUE(file) << "Could not open boost.jpg locally";

  std::vector<char> memblock;
  std::size_t size = readfile(file, memblock);

  ASSERT_NE(0, size);
  EXPECT_EQ(size, body(response_).size());

  auto diff_pos =
      std::mismatch(memblock.begin(), memblock.end(), body_.begin());
  EXPECT_EQ(boost::numeric_cast<std::size_t>(diff_pos.first - memblock.begin()),
            size);
}

TYPED_TEST(HTTPClientTest, FileNotFoundTest) {
  // Request for a non existing file.
  // Ensure that we have a body even in the presence of an error response
  using namespace boost::network;
  using client = TypeParam;

  typename client::request req(base_url + "/file_not_found");
  client c;
  typename client::response r = c.get(req);

  EXPECT_NE(0, body(r).size());
}

TYPED_TEST(HTTPClientTest, HeadTest) {
  using namespace boost::network;
  using client = TypeParam;
  typename client::request request_(base_url + "/test.xml");
  client client_;
  typename client::response response_ = client_.head(request_);
  ASSERT_NE(0, headers(response_).count("Content-Length"));
  auto range = headers(response_)["Content-Length"];
  EXPECT_EQ("Content-Length", std::begin(range)->first);
  EXPECT_EQ("113", std::begin(range)->second);
  EXPECT_EQ(0, body(response_).size());
}
