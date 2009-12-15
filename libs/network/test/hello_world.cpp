// Copyright 2009 (c) Tarro, Inc.
// Copyright 2009 (c) Dean Michael Berris <mikhailberis@gmail.com>
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdlib>
#include <boost/network/protocol/http/server.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/lexical_cast.hpp>
#include <string>

namespace http = boost::network::http;
using boost::assign::list_of;
using boost::lexical_cast;
using std::string;

struct hello_world {
    void operator()(http::request_pod const & request, http::reply & reply) {
        reply.content = "Hello, World!";
        http::request_header content_length = { "Content-Length", lexical_cast<string>(reply.content.size()) };
        http::request_header content_type = { "Content-Type", "text/plain" };
        reply.headers = list_of(content_length)(content_type);
        reply.status = http::reply::ok;
        assert(reply.status == http::reply::ok);
        assert(reply.headers.size() == 2);
        assert(reply.content == "Hello, World!");
    }
    void log(...) {
        // do nothing
    }
};

typedef http::server<hello_world> server;

int main(int argc, char * argv[]) {
    hello_world handler;
    server server_("127.0.0.1", "8000", handler);
    server_.run();
    return EXIT_SUCCESS;
}
