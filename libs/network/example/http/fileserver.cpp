// Copyright 2010 Dean Michael Berris.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <memory>
#include <thread>
#include <boost/network/include/http/server.hpp>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <iostream>
#include <cassert>

namespace http = boost::network::http;
namespace utils = boost::network::utils;

struct file_server;
typedef http::server<file_server> server;

struct file_cache {
  typedef std::map<std::string, std::pair<void *, std::size_t> > region_map;
  typedef std::map<std::string, std::vector<server::response_header> > meta_map;

  std::string doc_root_;
  region_map regions;
  meta_map file_headers;
  std::mutex cache_mutex;

  explicit file_cache(std::string doc_root) : doc_root_(std::move(doc_root)) {}

  ~file_cache() throw() {
    for (auto &region : regions) {
      munmap(region.second.first, region.second.second);
    }
  }

  bool has(std::string const &path) {
    std::unique_lock<std::mutex> lock(cache_mutex);
    return regions.find(doc_root_ + path) != regions.end();
  }

  bool add(std::string const &path) {
    std::unique_lock<std::mutex> lock(cache_mutex);
    std::string real_filename = doc_root_ + path;
    if (regions.find(real_filename) != regions.end()) return true;
#ifdef O_NOATIME
    int fd = open(real_filename.c_str(), O_RDONLY | O_NOATIME | O_NONBLOCK);
#else
    int fd = open(real_filename.c_str(), O_RDONLY | O_NONBLOCK);
#endif
    if (fd == -1) return false;
    off_t size = lseek(fd, 0, SEEK_END);
    if (size == -1) {
      close(fd);
      return false;
    }
    void *region = mmap(0, size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (region == MAP_FAILED) {
      close(fd);
      return false;
    }

    regions.insert(std::make_pair(real_filename, std::make_pair(region, size)));
    static server::response_header common_headers[] = {
        {"Connection", "close"},
        {"Content-Type", "x-application/octet-stream"},
        {"Content-Length", "0"}};
    std::vector<server::response_header> headers(common_headers,
                                                 common_headers + 3);
    headers[2].value = std::to_string(size);
    file_headers.insert(std::make_pair(real_filename, headers));
    close(fd);
    return true;
  }

  std::pair<void *, std::size_t> get(std::string const &path) {
    std::unique_lock<std::mutex> lock(cache_mutex);
    region_map::const_iterator region = regions.find(doc_root_ + path);
    if (region != regions.end())
      return region->second;
    else
      return std::pair<void *, std::size_t>(0, 0);
  }

  boost::iterator_range<std::vector<server::response_header>::iterator> meta(
      std::string const &path) {
    std::unique_lock<std::mutex> lock(cache_mutex);
    static std::vector<server::response_header> empty_vector;
    auto headers = file_headers.find(doc_root_ + path);
    if (headers != file_headers.end()) {
      auto begin = headers->second.begin(), end = headers->second.end();
      return boost::make_iterator_range(begin, end);
    } else
      return boost::make_iterator_range(empty_vector);
  }
};

struct connection_handler : std::enable_shared_from_this<connection_handler> {
  explicit connection_handler(file_cache &cache) : file_cache_(cache) {}

  void operator()(std::string const &path, server::connection_ptr connection,
                  bool serve_body) {
    bool ok = file_cache_.has(path);
    if (!ok) ok = file_cache_.add(path);
    if (ok) {
      send_headers(file_cache_.meta(path), connection);
      if (serve_body) send_file(file_cache_.get(path), 0, connection);
    } else {
      not_found(path, connection);
    }
  }

  void not_found(std::string const &, server::connection_ptr connection) {
    static server::response_header headers[] = {{"Connection", "close"},
                                                {"Content-Type", "text/plain"}};
    connection->set_status(server::connection::not_found);
    connection->set_headers(boost::make_iterator_range(headers, headers + 2));
    connection->write("File Not Found!");
  }

  template <class Range>
  void send_headers(Range const &headers, server::connection_ptr connection) {
    connection->set_status(server::connection::ok);
    connection->set_headers(headers);
  }

  void send_file(std::pair<void *, std::size_t> mmaped_region, off_t offset,
                 server::connection_ptr connection) {
    // chunk it up page by page
    std::size_t adjusted_offset = offset + 4096;
    off_t rightmost_bound = std::min(mmaped_region.second, adjusted_offset);
    auto self = this->shared_from_this();
    connection->write(
        boost::asio::const_buffers_1(
            static_cast<char const *>(mmaped_region.first) + offset,
            rightmost_bound - offset),
        [=](boost::system::error_code const &ec) {
          self->handle_chunk(mmaped_region, rightmost_bound, connection, ec);
        });
  }

  void handle_chunk(std::pair<void *, std::size_t> mmaped_region, off_t offset,
                    server::connection_ptr connection,
                    boost::system::error_code const &ec) {
    assert(offset >= 0);
    if (!ec && static_cast<std::size_t>(offset) < mmaped_region.second)
      send_file(mmaped_region, offset, connection);
  }

  file_cache &file_cache_;
};

struct input_consumer : public std::enable_shared_from_this<input_consumer> {
  // Maximum size for incoming request bodies.
  static constexpr std::size_t MAX_INPUT_BODY_SIZE = 2 << 16;

  explicit input_consumer(std::shared_ptr<connection_handler> h,
                          server::request r)
      : request_(std::move(r)), handler_(std::move(h)), content_length_{0} {
    for (const auto &header : request_.headers) {
      if (boost::iequals(header.name, "content-length")) {
        content_length_ = std::stoul(header.value);
        std::cerr << "Content length: " << content_length_ << '\n';
        break;
      }
    }
  }

  void operator()(server::connection::input_range input, boost::system::error_code ec,
                  std::size_t bytes_transferred,
                  server::connection_ptr connection) {
    std::cerr << "Callback: " << bytes_transferred << "; ec = " << ec << '\n';
    if (ec == boost::asio::error::eof) return;
    if (!ec) {
      if (empty(input))
        return (*handler_)(request_.destination, connection, true);
      request_.body.insert(request_.body.end(), boost::begin(input),
                           boost::end(input));
      if (request_.body.size() > MAX_INPUT_BODY_SIZE) {
        connection->set_status(server::connection::bad_request);
        static server::response_header error_headers[] = {
            {"Connection", "close"}};
        connection->set_headers(
            boost::make_iterator_range(error_headers, error_headers + 1));
        connection->write("Body too large.");
        return;
      }
      std::cerr << "Body: " << request_.body << '\n';
      if (request_.body.size() == content_length_)
        return (*handler_)(request_.destination, connection, true);
      std::cerr << "Scheduling another read...\n";
      auto self = this->shared_from_this();
      connection->read([self](server::connection::input_range input,
                              boost::system::error_code ec, std::size_t bytes_transferred,
                              server::connection_ptr connection) {
        (*self)(input, ec, bytes_transferred, connection);
      });
    }
  }

  server::request request_;
  std::shared_ptr<connection_handler> handler_;
  size_t content_length_;
};

struct file_server {
  explicit file_server(file_cache &cache) : cache_(cache) {}

  void operator()(server::request const &request,
                  server::connection_ptr connection) {
    if (request.method == "HEAD") {
      std::shared_ptr<connection_handler> h(new connection_handler(cache_));
      (*h)(request.destination, connection, false);
    } else if (request.method == "GET") {
      std::shared_ptr<connection_handler> h(new connection_handler(cache_));
      (*h)(request.destination, connection, true);
    } else if (request.method == "PUT" || request.method == "POST") {
      auto h = std::make_shared<connection_handler>(cache_);
      auto c = std::make_shared<input_consumer>(h, request);
      connection->read([c](server::connection::input_range input,
                           boost::system::error_code ec, std::size_t bytes_transferred,
                           server::connection_ptr connection) {
        (*c)(input, ec, bytes_transferred, connection);
      });
    } else {
      static server::response_header error_headers[] = {
          {"Connection", "close"}};
      connection->set_status(server::connection::not_supported);
      connection->set_headers(
          boost::make_iterator_range(error_headers, error_headers + 1));
      connection->write("Method not supported.");
    }
  }

  file_cache &cache_;
};

int main(int, char *[]) {
  try {
    file_cache cache(".");
    file_server handler(cache);
    server::options options(handler);
    server instance(options.thread_pool(std::make_shared<utils::thread_pool>(4))
                        .address("0.0.0.0")
                        .port("8000"));
    instance.run();
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
}
