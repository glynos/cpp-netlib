#ifndef BOOST_NETWORK_PROTOCOL_HTTP_DETAIL_CONNECTION_HELPER_20091217
#define BOOST_NETWORK_PROTOCOL_HTTP_DETAIL_CONNECTION_HELPER_20091217

//          Copyright Dean Michael Berris 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/version.hpp>
#include <boost/foreach.hpp>
#include <boost/network/protocol/http/traits/connection_keepalive.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/network/traits/string.hpp>

namespace boost { namespace network { namespace http { namespace detail {

    template <class Tag, unsigned version_major, unsigned version_minor>
    struct connection_helper {
        protected:

        typedef typename string<Tag>::type string_type;

        void create_request(boost::asio::streambuf & request_buffer, string_type const & method, basic_request<Tag> request_) const {
            // TODO make this use Boost.Karma instead of an ad-hoc implementation
            std::ostream request_stream(&request_buffer);

            request_stream
                << method << " ";

            if (request_.path().empty() || request_.path()[0] != '/')
                request_stream << '/';

            request_stream
                << request_.path()
                ;

            if (!request_.query().empty())
                request_stream
                    << '?'
                    << request_.query()
                    ;

            if (!request_.anchor().empty())
                request_stream
                    << '#'
                    << request_.anchor()
                    ;

            request_stream << " HTTP/" << version_major << '.' << version_minor << "\r\n"
                << "Host: " << request_.host() << "\r\n"
                << "Accept: */*\r\n";

            if (version_major == 1 && version_minor == 1)
                request_stream 
                    << "Accept-Encoding: identity;q=1.0, *;q=0\r\n"; // only accept identity encoding

            typename headers_range<http::basic_request<Tag> >::type range = headers(request_);
            BOOST_FOREACH(typename headers_range<http::basic_request<Tag> >::type::value_type const & header, range) {
                request_stream << header.first << ": " << header.second << "\r\n";
            };

            range = headers(request_)["user-agent"];
            if (empty(range)) request_stream << "User-Agent: cpp-netlib/" << BOOST_NETLIB_VERSION << "\r\n";

            if (!connection_keepalive<Tag>::value) {
                request_stream
                    << "Connection: close\r\n";
            }
            request_stream
                << "\r\n";

            string_type body_ = body(request_);
            if (!body_.empty())
                request_stream << body_;
        }
        
    };

} // namespace detail

} // namespace http

} // namespace network

} // namespace boost

#endif // BOOST_NETWORK_PROTOCOL_HTTP_DETAIL_CONNECTION_HELPER_20091217

