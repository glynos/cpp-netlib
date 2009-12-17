// Copyright 2009 (c) Tarro, Inc.
// Copyright 2009 (c) Dean Michael Berris <mikhailberis@gmail.com>
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/config/warning_disable.hpp>
#include <boost/network/protocol/http/server.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/lexical_cast.hpp>
#include <string>

namespace http = boost::network::http;
using boost::assign::list_of;
using boost::lexical_cast;
using std::string;

struct hello_world;
typedef http::server<hello_world> server;

struct hello_world {
    void operator()(server::request const & request, server::response & response) {
        response.content = "Hello, World!";
        http::request_header content_length = { "Content-Length", lexical_cast<string>(response.content.size()) };
        http::request_header content_type = { "Content-Type", "text/plain" };
        response.headers = list_of(content_length)(content_type);
        response.status = server::response::ok;
        assert(response.status == server::response::ok);
        assert(response.headers.size() == 2);
        assert(response.content == "Hello, World!");
    }
    void log(...) {
        // do nothing
    }
};

int main(int argc, char * argv[]) {
    hello_world handler;
    server server_("127.0.0.1", "8000", handler);
    server_.run();
    return EXIT_SUCCESS;
}
