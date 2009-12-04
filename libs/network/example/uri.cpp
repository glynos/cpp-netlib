
// Copyright Dean Michael Berris 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/uri.hpp>
#include <string>
#include <iostream>

using namespace std;
using namespace boost::network::uri;

int main(int argc, char * argv[]) {
    string input;
    cout << "Please enter a URI to parse: ";
    cin >> input;

    uri uri_(input);
    cout << "You've entered " 
        << (valid(uri_)? string("a valid") : string("an invalid"))
        << " URI!" << endl;

    http::uri http_uri_(input);
    cout << "It's also "
        << (valid(http_uri_)? string("a valid HTTP URI") : string("an invalid HTTP URI."))
        << "!" << endl;

    return EXIT_SUCCESS;
}

