
// Copyright 2008, 2014 Dean Michael Berris <dberris@google.com>
// Copyright 2014 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//[ http_client_main
/*`
  This application takes a URL as a command line argument and prints
  the resource to the console.
*/
#include <boost/program_options.hpp>
#include <boost/network/protocol/http.hpp>
#include <string>
#include <utility>
#include <iostream>
#include "cxxopts.hpp"

int main(int argc, char* argv[]) {
  using namespace boost::network;

  cxxopts::Options options(argv[0], "Allowed options");
  options.add_options()
    ("h,help", "produce help message")
    ("H,headers", "print headers")
    ("S,status", "print status and message")
    ("s,source", "source URL", cxxopts::value<std::string>())
    ;

  options.parse_positional(std::vector<std::string>{"source"});
  options.parse(argc, argv);

  if (options.count("help")) {
    std::cout << options.help({"", "Group"}) << std::endl;
    return EXIT_SUCCESS;
  }

  if (options.count("source") < 1) {
    std::cout << "Error: Source URL required." << std::endl;
    std::cout << options.help({"", "Group"}) << std::endl;
    return EXIT_FAILURE;
  }

  std::string source = options["source"].as<std::string>();
  bool show_headers = options.count("headers") ? true : false;
  bool show_status = options.count("status") ? true : false;

  http::client::request request(source);
  http::client::string_type destination_ = host(request);

  request << ::boost::network::header("Connection", "close");
  http::client::options client_options;
  client_options.follow_redirects(true);
  http::client client(client_options);
  http::client::response response = client.get(request);

  if (show_status)
    std::cout << status(response) << " " << status_message(response)
              << std::endl;

  if (show_headers) {
    auto headers_ = response.headers();
    for (auto const& header : headers_) {
      std::cout << header.first << ": " << header.second << std::endl;
    }
    std::cout << std::endl;
  }

  std::cout << body(response);
  return EXIT_SUCCESS;
}
//]
