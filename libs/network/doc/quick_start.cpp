//            Copyright (c) Glyn Matthews 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


//[ quick_start_main
/*`
  Here is an example of __cnl__ at it's absolute simplest.  It
  downloads a file over HTTP from a URL given as a command line
  argument and prints the response to the console.
 */

#include <boost/network.hpp>
#include <iostream>

int
main(int argc, char *argv[]) {
    namespace http = boost::network::http;
    
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " url" << std::endl;
        return 1;
    }
    
    try {
        /*<< Create the client >>*/
        http::client client;
        /*<< Create a request using a URI supplied on the command
             line >>*/
        http::request request(argv[1]);
        /*<< Get a response >>*/
        http::response response = client.get(request);

        /*<< Iterate through the response headers and print them to
             the console >>*/
        headers_range<http::response>::type hdrs = headers(response);
        boost::range_iterator<headers_range<http::response>::type>::type
            it = boost::begin(hdrs), end = boost::end(hdrs);
        for (; it != end; ++it) {
            std::cout << it->first << ": " << it->second << std::endl;
        }

        /*<< Print the response body to the console >>*/
        std::cout << http::body(response) << std::endl;
    }
    catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
//]
