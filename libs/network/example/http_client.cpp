
//          Copyright Dean Michael Berris 2008.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <boost/program_options.hpp>
#include <boost/network/protocol/http.hpp>
#include <string>
#include <iostream>

namespace po = boost::program_options;
using namespace std;

int main(int argc, char * argv[]) {
	po::options_description options("Allowed options");
	string output_filename, source;
	bool show_headers;
	options.add_options()
		("help,h", "produce help message")
		("headers,H", "print headers")
		("source,s", po::value<string>(&source), "source URL")
		;

	po::positional_options_description positional_options;
	positional_options.add("source", 1);
	po::variables_map vm;
	try {
		po::store(po::command_line_parser(argc, argv).options(options).positional(positional_options).run(),
			vm);
		po::notify(vm);
	} catch(exception & e) {
		cout << "Error: " << e.what() << endl;
		cout << options << endl;
		return EXIT_FAILURE;
	};

	if (vm.count("help")) {
		cout << options << endl;
		return EXIT_SUCCESS;
	};

	if (vm.count("source") < 1) {
		cout << "Error: Source URL required." << endl;
		cout << options << endl;
		return EXIT_FAILURE;
	};

	show_headers = vm.count("headers") ? true : false ;

	try {
		using namespace boost::network;
		
		http::request request(source);
		http::client client;
		http::response response;
		response = client.get(request);

		if (show_headers) {
			headers_range<http::response>::type headers_ = headers(response);
			boost::range_iterator<headers_range<http::response>::type>::type header, past_end;
			header = begin(headers_);
			past_end = end(headers_);
			while (header != past_end) {
				cout << header->first << ": " << header->second << endl;
				++header;
			};
			cout << endl;
		};

		cout << body(response) << endl;
		
	} catch (exception & e) {
		cout << "Error: " << e.what() << endl;
		return EXIT_FAILURE;
	};

	return EXIT_SUCCESS;
}
