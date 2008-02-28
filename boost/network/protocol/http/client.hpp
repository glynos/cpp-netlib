
//          Copyright Dean Michael Berris 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef __NETWORK_PROTOCOL_HTTP_CLIENT_20070908_1_HPP__
#define __NETWORK_PROTOCOL_HTTP_CLIENT_20070908_1_HPP__

#include <boost/network/protocol/http/response.hpp>
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include <sstream>
#include <string>

namespace boost { namespace network { namespace http {

    template <class tag, unsigned int version_major = 1, unsigned version_minor = 0>
    class basic_client {

    public:
        
        response const get (basic_request<tag> const & request_) {
            // use Asio's iostream interface to perform
            // a synchronous request for data via HTTP
            // and then craft a response object to be returned
            // based on the headers received, and the contents
            using namespace boost::asio;

            ip::tcp::iostream socket_stream(request_.host(), boost::lexical_cast<typename tag::str_type>(request_.port()), ip::tcp::resolver_query::numeric_service);
            socket_stream 
                << "GET /" 
                << request_.path()
                ;
                
            if (request_.query() != "")
                socket_stream
                    << '?'
                    << request_.query()
                    ;

            if (request_.anchor() != "")
                socket_stream
                    << '#'
                    << request_.anchor()
                    ;

            socket_stream << " HTTP/" << version_major << '.' << version_minor << "\r\n"
                << "Host: " << request_.host() << "\r\n\r\n";

            socket_stream << std::flush;
            
            response response_;
            typename tag::ostringstream_type body_stream;

            while (!socket_stream.eof()) {
                typename tag::str_type line;
                std::getline(socket_stream, line);
                if (line.size() == 0u) {
                    break;
                };
                response_
                    << 
                    header( line.substr(0, line.find(':')) , 
                    line.find(' ') == tag::str_type::npos ?
                    typename tag::str_type()
                    : line.substr(line.find(' ') + 1));
            };

            while (!socket_stream.eof()) {
                typename tag::str_type line;
                std::getline(socket_stream, line);
                body_stream << line;  
            };

            response_ << body(body_stream.str());
            return response_;
        };

    };

    typedef basic_client<tags::default_, 1, 0> client;

}; // namespace http

}; // namespace network

}; // namespace boost

#endif // __NETWORK_PROTOCOL_HTTP_CLIENT_20070908_1_HPP__
