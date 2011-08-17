// Copyright 2009 (c) Tarro, Inc.
// Copyright 2009 (c) Dean Michael Berris <mikhailberis@gmail.com>
// Copyright 2011 (c) Matt Leisinger <leisinger@gmail.com>
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

//[ hello_world_server_main
/*`
  This is a part of the 'Hello World' example. It's used to
  demonstrate how easy it is to set up an HTTP server.  All we do in
  this example is create a request handler and run the server. This
  version of the server simply adds logging
 */
#include <iostream>

#include <boost/network/protocol/http/server.hpp>

#include <boost/log/common.hpp>
#include <boost/log/formatters.hpp>
#include <boost/log/filters.hpp>

#include <boost/log/utility/init/to_file.hpp>
#include <boost/log/utility/init/to_console.hpp>
#include <boost/log/utility/init/common_attributes.hpp>

#include <boost/log/attributes.hpp>
#include <boost/log/attributes/timer.hpp>

namespace http = boost::network::http;
namespace logging = boost::log;
namespace fmt = boost::log::formatters;
namespace flt = boost::log::filters;
namespace sinks = boost::log::sinks;
namespace attrs = boost::log::attributes;
namespace src = boost::log::sources;
namespace keywords = boost::log::keywords;

/*<< Defines the server. >>*/
struct hello_world;
typedef http::server<hello_world> server;

/*<< Defines the request handler.  It's a class that defines two
     functions, `operator()` and `log()` >>*/
struct hello_world {
    /*<< This is the function that handles the incoming request. >>*/
    void operator() (server::request const &request,
                     server::response &response) {
        server::string_type ip = source(request);
        std::ostringstream data;
        data << "Hello, " << ip << "!";
        response = server::response::stock_reply(
            server::response::ok, data.str());
    }
    /*<< Using Boost.Log we'll simply log the number of errors >>*/
    void log(const char *error) {
        BOOST_LOG(lg) << "Error occured (" << error << "). Total number of errors reported is " << ++numErrors; 
    }

    /*<< Catch all for any other errors >>*/
    void log(...) {
        BOOST_LOG(lg) << "Unknown error occured. Total number of error reported is " << ++numErrors; 
    }
    private:
        src::logger_mt lg;
        uint32_t numErrors;
        
};

/*<< Sets up two logger sinks, one to the console and the other to a file >>*/
void init_logging()
{
    /*<< Setup the console logger >>*/
    logging::init_log_to_console(std::clog, keywords::format = "%TimeStamp%: %_%");

    /*<< Setup the file logger >>*/
    logging::init_log_to_file
    (
        "hello_world_server.log",
        keywords::format = fmt::format("[%1%] %2%")
            % fmt::date_time("TimeStamp", std::nothrow)
            % fmt::message(),
        keywords::auto_flush = true
    );

    logging::add_common_attributes();
}

int main(int argc, char * argv[]) {
    
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " address port" << std::endl;
        return 1;
    }

    init_logging();
    src::logger_mt lg;    

    try {
        /*<< Creates the request handler. >>*/
        hello_world handler;
        /*<< Creates the server. >>*/
        server server_(argv[1], argv[2], handler);
        /*<< Write to the log source >>*/
        BOOST_LOG(lg) << "Starting HTTP Hello World Server...";
        /*<< Runs the server. >>*/
        server_.run();
    }
    catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
//]
