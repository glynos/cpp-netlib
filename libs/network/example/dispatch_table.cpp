
//          Copyright Divye Kapoor 2008.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <boost/algorithm/string/case_conv.hpp>
#include <boost/network/protocol/http.hpp>
#include <string>
#include <map>
#include <exception>
#include <iostream>

using namespace boost::network;
using std::cerr;
using std::cout;
using std::cin;
using std::endl;


namespace boost { namespace network { namespace http { 
		std::map<std::string, 
			http::response const (http::client::*)(http::request const &)> dispatch_table;
		
		
		namespace impl {
			struct init_dispatch_table {
							
				init_dispatch_table() {
					http::dispatch_table["GET"] = &http::client::get;
					http::dispatch_table["POST"] = &http::client::post;
					http::dispatch_table["HEAD"] = &http::client::head;
					http::dispatch_table["PUT"] = &http::client::put;
					http::dispatch_table["DELETE"] = &http::client::delete_;
				}
				
			};
		
			struct unsupported_method_exception : public std::logic_error {
				unsupported_method_exception(std::string const &method) : 
					std::logic_error( (std::string("Method ") + method + std::string(" is not a valid HTTP method.")).c_str() ) {
				}
			};
		} // namespace impl
		
		typedef impl::unsupported_method_exception unsupported_method;
		
		http::response perform(http::client & client_, std::string method, http::request const & request_) {
			static impl::init_dispatch_table dummy;
			
			boost::to_upper(method);
			if (http::dispatch_table.find(method) == http::dispatch_table.end())
				throw http::unsupported_method(method);
			
			return (client_.*http::dispatch_table[method])(request_);
		}
} // namespace http
} // namespace network
} // namespace boost


// Prerequisite: The Python Server at tests/server/cgi_server.py must be running to serve the http request
int main() {
	http::request request_("http://localhost:8000/cgi-bin/requestinfo.py");
	http::client client_;
	http::response response_;
	
	// Construct request object here
	
	try {
		http::perform(client_, "GET", request_);
		http::perform(client_, "POST", request_); 
		http::perform(client_, "UnsupportedMethod", request_);
	} catch(http::unsupported_method const &e) {
		cerr << e.what() << endl;
	} catch(boost::system::system_error const &e) {
		cerr << e.what() << endl;
	}

	return 0;
}
