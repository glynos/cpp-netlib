// Copyright 2018 Martin Trenkmann
// Based on cpp-netlib/libs/network/example/http/fileserver.cpp
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// This example implements a minimal echo server that receives plain text via
// POST requests and sends the same text back to the client (with an additional
// newline appended for nicer output).  Since the server uses asynchronous I/O,
// the body data is not available through request.body, but must be read using
// connection->read(callback).
//
// This code is minimal, it does not...
// - handle any kinds of errors,
// - look at the URI path (request.destination),
// - handle request methods other than POST (request.method),
// - check for too large bodies (which make the server vulnerable to attacks),
// - model state, e.g. database connections (this type of context should be
//   injected into the post_request_handler constructor).
//
// About handling large body data:
// According to https://tools.ietf.org/html/rfc2616#section-8.2.3, HTTP clients
// may send a "Expect: 100-continue" header before sending large body data in
// order to negotiate whether the server is willing to accept the data.  If the
// server agrees, it should send an interim response with a "100 Continue"
// status code.  The client then should start sending the data and upon
// completion, the server should send a final response including a status code
// and possibly headers and body data.
//
// About cpp-netlib 0.13 and the "Expect: 100-continue" header:
// When a client sends a POST request with a "Expect: 100-continue" header, the
// callback passed into connection->read() gets triggered with an empty chunk of
// data.  As described before, the server should then send an "100 Continue"
// interim response, but this seems to be not supported by the library at the
// moment, because trying to set and send a status code on the connection object
// more than once throws an exception.  However, since clients are allowed to
// send data even though they never received a "100 Continue", the server should
// continue reading data from the connection, by calling connection->read()
// again, until the expected number of bytes, based on the "Content-Length"
// header, has been received.
//
// About curl and the "Expect: 100-continue" header:
// According to https://developer.mozilla.org/en-US/docs/Web/HTTP/Headers/Expect
// no common browsers make use of the "Expect" header, but some other clients
// such as curl do so by default.  In fact, curl sends a "Expect: 100-continue"
// header with POST requests that contain at least 1KiB of data (see example
// session below).  Fortunately, curl also starts sending the data if it does
// not receive a "100 Continue" status code after a timeout of 1 second, which
// makes it "compatible" with servers that cannot send interim responses such as
// cpp-netlib 0.13.
//
// The timeout in curl is configurable via "--expect100-timeout <seconds>"
// command line argument.  The minimum value seems to be 1 second.
//
// Example session:
// Two POST requests are sent to the echo server using curl.  The first one with
// data less than 1KiB, the second one with data more than 1KiB which shows the
// use of the "Expect: 100-continue" header.
//
// The "lorem" command generates lorem ipsum text.  It can be installed via
// "sudo apt-get install libtext-lorem-perl" on Debian-based systems.
//
// 1. Sending less than 1KiB
// -------------------------
//
// $ curl -v -H 'Content-Type: text/plain' -d "$(lorem -s 3 | fold)" http://localhost:8000
// * Rebuilt URL to: http://localhost:8000/
// *   Trying 127.0.0.1...
// * Connected to localhost (127.0.0.1) port 8000 (#0)
// > POST / HTTP/1.1
// > Host: localhost:8000
// > User-Agent: curl/7.47.0
// > Accept: */*
// > Content-Type: text/plain
// > Content-Length: 155
// >
// * upload completely sent off: 155 out of 155 bytes
// < HTTP/1.1 200 OK
// < Content-Type: text/plain
// < Content-Length: 156
// <
// Iusto autem omnis necessitatibus quia omnis nemo sequi. Occaecati est explicabo
// qui placeat ipsa debitis fugit sit. Quasi sequi in est eius qui molestiae.
// * Connection #0 to host localhost left intact
//
// SERVER OUTPUT
//
// Host: localhost:8000
// User-Agent: curl/7.47.0
// Accept: */*
// Content-Type: text/plain
// Content-Length: 155
// Chunk size: 155
//
// 2. Sending more than 1KiB
// -------------------------
//
// $ curl -v -H 'Content-Type: text/plain' -d "$(lorem -s 30 | fold)" http://localhost:8000
// * Rebuilt URL to: http://localhost:8000/
// *   Trying 127.0.0.1...
// * Connected to localhost (127.0.0.1) port 8000 (#0)
// > POST / HTTP/1.1
// > Host: localhost:8000
// > User-Agent: curl/7.47.0
// > Accept: */*
// > Content-Type: text/plain
// > Content-Length: 1324
// > Expect: 100-continue
// >
// * Done waiting for 100-continue
// * We are completely uploaded and fine
// < HTTP/1.1 200 OK
// < Content-Type: text/plain
// < Content-Length: 1325
// <
// Fugit quo aliquid in et consectetur sed id. Aliquam dolor optio labore sit autem
// . Culpa at omnis et consectetur minima nostrum sed. Veniam similique dolorum des
// erunt aut et et aut quo. Laudantium nesciunt est repellat. Dolores adipisci alia
// s dicta dicta. Impedit porro pariatur quisquam sit ex ducimus a. Consequatur ius
// to possimus in sint nesciunt molestiae fugiat et. Est praesentium quos quam libe
// ro vel nostrum placeat consequuntur. Tempora nihil aut aliquam. Atque ab sunt ut
//  sed quo ut. Quia qui omnis non. In laboriosam possimus laboriosam consequatur v
// el dolores. Reprehenderit totam quis dolore debitis ullam. Aut iure omnis invent
// ore quaerat aut veniam vel magni. Temporibus voluptatibus accusamus qui facilis
// at aut. Voluptatem provident incidunt officia. Quos quo autem quae illo. Modi am
// et quis eveniet. Nemo tenetur quia unde. Velit molestiae laborum eum. Repellat m
// olestias eos eos accusantium dolorem molestias pariatur ex. Nihil dolorum possim
// us ut ut beatae. Quia nam sit aut voluptatum maiores quibusdam id aliquid. Nulla
//  numquam rem quo doloremque ut ut. Aspernatur accusamus illo illo dolores repudi
// andae dicta reiciendis. Quis laborum magni et incidunt nihil. Ea quia consequunt
// ur quos minima aut veniam ratione sed. Ea deleniti accusamus est quo nisi. Quia
// quibusdam et aut reiciendis.
// * Connection #0 to host localhost left intact
//
// SERVER OUTPUT
//
// Host: localhost:8000
// User-Agent: curl/7.47.0
// Accept: */*
// Content-Type: text/plain
// Content-Length: 1324
// Expect: 100-continue
// Chunk size: 0
// Chunk size: 1024
// Chunk size: 300

#include <boost/asio/signal_set.hpp>
#include <boost/network/protocol/http/server.hpp>
#include <cassert>
#include <memory>
#include <string>
#include <vector>

namespace http = boost::network::http;

struct request_handler;
using echo_server = http::server<request_handler>;

struct post_request_handler
    : public std::enable_shared_from_this<post_request_handler> {
  explicit post_request_handler(const echo_server::request& request)
      : content_length_(0) {
    for (const auto& header : request.headers) {
      std::cout << header.name << ": " << header.value << '\n';
      if (boost::iequals(header.name, "content-length")) {
        content_length_ = std::stoul(header.value);
      }
    }
  }

  void operator()(echo_server::connection::input_range chunk,
                  boost::system::error_code ec, size_t chunk_size,
                  echo_server::connection_ptr connection) {
    assert(chunk.size() == chunk_size);
    std::cout << "Chunk size: " << chunk_size << '\n';

    if (ec) {
      std::cerr << "Error code: " << ec << '\n';
      return;
    }

    body_.append(chunk.begin(), chunk.end());
    if (body_.size() < content_length_) {
      auto self = this->shared_from_this();
      connection->read([self](echo_server::connection::input_range chunk,
                              boost::system::error_code ec, size_t chunk_size,
                              echo_server::connection_ptr connection) {
        (*self)(chunk, ec, chunk_size, connection);
      });
      return;
    }

    body_.push_back('\n');
    std::vector<echo_server::response_header> headers;
    headers.push_back({"Content-Type", "text/plain"});
    headers.push_back({"Content-Length", std::to_string(body_.size())});
    connection->set_status(echo_server::connection::ok);
    connection->set_headers(headers);
    connection->write(body_);
  }

 private:
  size_t content_length_;
  std::string body_;
};

struct request_handler {
  void operator()(const echo_server::request& request,
                  echo_server::connection_ptr connection) {
    if (request.method == "POST") {
      auto h = std::make_shared<post_request_handler>(request);
      connection->read([h](echo_server::connection::input_range chunk,
                           boost::system::error_code ec, size_t chunk_size,
                           echo_server::connection_ptr connection) {
        (*h)(chunk, ec, chunk_size, connection);
      });
    }
  }
};

int main() {
  try {
    request_handler handler;
    auto io_service = std::make_shared<boost::asio::io_service>();
    echo_server server(
        echo_server::options(handler).io_service(io_service).port("8000"));
    server.run();

    // Clean shutdown when pressing Ctrl+C.
    boost::asio::signal_set signals(*io_service, SIGINT, SIGTERM);
    signals.async_wait([&server](const boost::system::error_code& ec,
                                 int /* signal_number */) {
      if (!ec) {
        server.stop();
      }
    });
    return EXIT_SUCCESS;
  } catch (const std::exception& error) {
    std::cerr << error.what() << std::endl;
    return EXIT_FAILURE;
  }
}
