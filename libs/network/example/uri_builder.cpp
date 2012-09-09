//            Copyright (c) Glyn Matthews 2011, 2012.
// Copyright 2012 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#include <iostream>
#include <network/uri.hpp>
#include <network/uri/uri_io.hpp>

int
main(int argc, char *argv[]) {

    network::uri url;
    url << network::scheme("http")
        << network::host("www.github.com")
        << network::path("/cpp-netlib");
    std::cout << url << std::endl;

    return 0;
}
