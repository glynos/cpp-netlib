
// Copyright 2010 Dean Michael Berris. 
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_TEST_MODULE HTTP Asynchronous Server Tests

#include <boost/config/warning_disable.hpp>
#include <boost/network/include/http/server.hpp>
#include <boost/network/utils/thread_pool.hpp>

namespace http = boost::network::http;
namespace utils = boost::network::utils;

struct async_hello_world;
typedef http::async_server<async_hello_world> server;

struct async_hello_world {
    void operator()(server::request const & request, server::connection & connection) {
        server::response_header headers[] = {
            {"Connection", "close"}
            , {"Content-Type", "text/plain"}
        };
        connection.set_headers(boost::make_iterator_range(headers, headers+2));
        if (request.method != "GET") {
            connection.set_status(server::connection::not_supported);
            connection.write("Unsupported method.");
        } else {
            connection.set_status(server::connection::ok);
            connection.write("Hello, World!");
        }
        connection.flush();
        connection.close();
    }
};

int main(int argc, char * argv[]) {
    utils::thread_pool thread_pool(2); 
    async_hello_world handler;
    server instance("127.0.0.1", "8000", handler, thread_pool);
    instance.run();
    return 0;
}

