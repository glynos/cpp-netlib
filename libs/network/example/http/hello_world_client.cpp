//            Copyright (c) Glyn Matthews 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


//[ hello_world_client_main
/*
  This is a part of the 'Hello World' example.

  See also `hello_world_server.cpp`.
 */
#include <boost/network.hpp>
#include <boost/network/uri/http/uri.hpp>
#include <iostream>


namespace http = boost::network::http;
namespace uri = boost::network::uri;


int
main(int argc, char *argv[]) {
    
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " url" << std::endl;
        return 1;
    }

    try {
        http::client client;
        const char *uri = argv[1];
        http::client::request request(uri);
        http::client::response response = client.get(request);
        std::cout << boost::network::body(response) << std::endl;
    }
    catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
//]
 
