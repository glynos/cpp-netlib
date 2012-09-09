//            Copyright (c) Glyn Matthews 2011, 2012.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <network/uri/uri.hpp>
#include <network/protocol/http/client.hpp>
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include <iostream>

// This example uses the Twitter Search API.
//
// https://dev.twitter.com/docs/using-search

int main(int argc, char *argv[]) {
    namespace http = boost::network::http;
    using namespace rapidjson;

    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <query>" << std::endl;
        return 1;
    }

    try {
        http::client client;

        network::uri base_uri("http://search.twitter.com/search.json");

        std::cout << "Searching Twitter for query: " << argv[1] << std::endl;
        network::uri search;
        search << base_uri << network::query("q", network::encoded(argv[1]));
        http::client::request request(search);
        http::client::response response = client.get(request);

        Document d;
        std::string body;
        response.get_body(body);
        if (!d.Parse<0>(body.c_str()).HasParseError()) {
			const Value &results = d["results"];
			for (SizeType i = 0; i < results.Size(); ++i)
			{
				const Value &user = results[i]["from_user_name"];
				const Value &text = results[i]["text"];
				std::cout << "From: " << user.GetString() << std::endl
						  << "  " << text.GetString() << std::endl
						  << std::endl;
            }
        }
    }
    catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
