#ifndef BOOST_NETWORK_PROTOCOL_HTTP_POLICIES_SIMPLE_CONNECTION_20091214
#define BOOST_NETWORK_PROTOCOL_HTTP_POLICIES_SIMPLE_CONNECTION_20091214

//          Copyright Dean Michael Berris 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/version.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/cstdint.hpp>
#include <boost/network/protocol/http/traits/resolver_policy.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>

namespace boost { namespace network { namespace http {

    template <class Tag, unsigned version_major, unsigned version_minor>
    struct simple_connection_policy : resolver_policy<Tag>::type {
        protected:

        typedef typename string<Tag>::type string_type;
        typedef typename resolver_policy<Tag>::type resolver_base;
        typedef typename resolver_base::resolver_type resolver_type;
        typedef function<typename resolver_base::resolver_iterator_pair(resolver_type &, string_type const &, string_type const &)> resolver_function_type;

        struct connection_impl {
            connection_impl(resolver_type & resolver, bool follow_redirect, string_type const & hostname, string_type const & port, resolver_function_type resolve) 
            : resolver_(resolver), socket_(resolver.io_service()), follow_redirect_(follow_redirect), resolve_(resolve)
            { }

            basic_response<Tag> send_request(string_type const & method, basic_request<Tag> request_, bool get_body) {
                basic_response<Tag> response_;
                do {
                    init_socket(request_.host(), lexical_cast<string_type>(request_.port()));
                    send_request_impl(method, request_);

                    response_ = basic_response<Tag>();
                    response_ << source(request_.host());

                    boost::asio::streambuf response_buffer;
                    read_status(response_, response_buffer);
                    read_headers(response_, response_buffer);
                    if (get_body) read_body(response_, response_buffer);

                    if (follow_redirect_) {
                        boost::uint16_t status = response_.status();
                        if (status >= 300 && status <= 307) {
                            typename headers_range<http::basic_response<Tag> >::type location_range = headers(response_)["Location"];
                            typename range_iterator<typename headers_range<http::basic_request<Tag> >::type>::type location_header = begin(location_range);
                            if (location_header != end(location_range)) {
                                request_.uri(location_header->second);
                            } else throw std::runtime_error("Location header not defined in redirect response.");
                        } else break;
                    } else break;
                } while(true);

                return response_;
            }

            private:

            resolver_type & resolver_;
            boost::asio::ip::tcp::socket socket_;
            bool follow_redirect_;
            resolver_function_type resolve_;

            void init_socket(string_type const & hostname, string_type const & port) {
                using boost::asio::ip::tcp;
                boost::system::error_code error = boost::asio::error::host_not_found;
                typename resolver_type::iterator endpoint_iterator, end;
                boost::tie(endpoint_iterator, end) = resolve_(resolver_, hostname, port);
                while (error && endpoint_iterator != end) {
                    socket_.close();
                    socket_.connect(
                        tcp::endpoint(
                            endpoint_iterator->endpoint().address()
                            , endpoint_iterator->endpoint().port()
                            )
                        , error
                        );
                    ++endpoint_iterator;
                }

                if (error)
                    throw boost::system::system_error(error);
            }

            void send_request_impl(string_type const & method, basic_request<Tag> const & request_) {
                boost::asio::streambuf request_buffer;
                create_request(request_buffer, method, request_);
                write(socket_, request_buffer);
            }

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

                typename headers_range<http::basic_request<Tag> >::type range = headers(request_);
                BOOST_FOREACH(typename headers_range<http::basic_request<Tag> >::type::value_type const & header, range) {
                    request_stream << header.first << ": " << header.second << "\r\n";
                };

                range = headers(request_)["user-agent"];
                if (empty(range)) request_stream << "User-Agent: cpp-netlib/" << BOOST_NETLIB_VERSION << "\r\n";

                request_stream
                    << "Connection: close\r\n\r\n";

                string_type body_ = body(request_);
                if (!body_.empty())
                    request_stream << body_;
            }

            void read_status(basic_response<Tag> & response_, boost::asio::streambuf & response_buffer) {
                boost::asio::read_until(socket_, response_buffer, "\r\n");
                std::istream response_stream(&response_buffer);
                string_type http_version;
                unsigned int status_code;
                string_type status_message;
                response_stream >> http_version
                    >> status_code;
                std::getline(response_stream, status_message);
                trim_left(status_message);
                trim_right_if(status_message, boost::is_space() || boost::is_any_of("\r"));

                if (!response_stream || http_version.substr(0, 5) != "HTTP/")
                    throw std::runtime_error("Invalid response");

                response_.version() = http_version;
                response_.status() = status_code;
                response_.status_message() = status_message;
            }

            void read_headers(basic_response<Tag> & response_, boost::asio::streambuf & response_buffer) {
                boost::asio::read_until(socket_, response_buffer, "\r\n\r\n");
                std::istream response_stream(&response_buffer);
                string_type header_line, name;
                while (std::getline(response_stream, header_line) && header_line != "\r") {
                    trim_right_if(header_line, boost::is_space() || boost::is_any_of("\r"));
                    typename string_type::size_type colon_offset;
                    if (header_line.size() && header_line[0] == ' ') {
                        assert(!name.empty());
                        if (name.empty())
                            throw std::runtime_error(
                                    std::string("Malformed header: ")
                                    + header_line
                                    );
                        response_
                            << header(name, trim_left_copy(header_line));
                    } else if ((colon_offset = header_line.find_first_of(':')) != string_type::npos) {
                        name = header_line.substr(0, colon_offset);
                        response_
                            << header(name, header_line.substr(colon_offset+2));
                    };
                };
            }

            void read_body(basic_response<Tag> & response_, boost::asio::streambuf & response_buffer) {
                typename ostringstream<Tag>::type body_stream;

                if (response_buffer.size() > 0)
                    body_stream << &response_buffer;

                boost::system::error_code error;
                while (boost::asio::read(socket_, response_buffer, boost::asio::transfer_at_least(1), error))
                    body_stream << &response_buffer;

                if (error != boost::asio::error::eof)
                    throw boost::system::system_error(error);

                response_ << body(body_stream.str());
            }

        };

        typedef boost::shared_ptr<connection_impl> connection_ptr;
        connection_ptr get_connection(resolver_type & resolver, basic_request<Tag> const & request_) {
            connection_ptr connection_(
                new connection_impl(
                    resolver
                    , follow_redirect_
                    , request_.host()
                    , lexical_cast<string_type>(request_.port())
                    , bind(
                        &simple_connection_policy<Tag,version_major,version_minor>::resolve,
                        this,
                        _1, _2, _3
                        )
                    )
                );
            return connection_;
        }

        simple_connection_policy(bool cache_resolved, bool follow_redirect) 
        : resolver_base(cache_resolved), follow_redirect_(follow_redirect) {}

        // member variables
        bool follow_redirect_;

    };

} // namespace http

} // namespace network

} // namespace boost


#endif // BOOST_NETWORK_PROTOCOL_HTTP_POLICIES_SIMPLE_CONNECTION_20091214

