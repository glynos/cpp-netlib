// Copyright 2009 (c) Tarro, Inc.
// Copyright 2009 (c) Dean Michael Berris <mikhailberis@gmail.com>
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

//[ hello_world_server_main
/*
  This is a part of the 'Hello World' example. It's used to
  demonstrate how easy it is to set up an HTTP server.

  See also `hello_world_client.cpp`.
 */
#include <cstdlib>
#include <boost/config/warning_disable.hpp>
#include <boost/network/protocol/http/server.hpp>
#include <boost/assign/list_of.hpp>
#include <string>

namespace http = boost::network::http;


struct hello_world;
typedef http::server<hello_world> server;


struct hello_world {
    void operator() (server::request const &request,
                     server::response &response) {
        response = server::response::stock_reply(server::response::ok, "Hello, World!");
    }
    void log(...) {
        // do nothing
    }
};


int main(int argc, char * argv[]) {
    
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " address port" << std::endl;
        return 1;
    }

    try {
        hello_world handler;
        server server_(argv[1], argv[2], handler);
        server_.run();
    }
    catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
//]
