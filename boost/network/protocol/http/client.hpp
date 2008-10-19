
//          Copyright Dean Michael Berris 2007-2008.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef __NETWORK_PROTOCOL_HTTP_CLIENT_20070908_1_HPP__
#define __NETWORK_PROTOCOL_HTTP_CLIENT_20070908_1_HPP__

#ifndef BOOST_NETLIB_VERSION
#define BOOST_NETLIB_VERSION "0.3"
#endif

#include <boost/network/protocol/http/message.hpp>
#include <boost/network/protocol/http/response.hpp>
#include <boost/network/protocol/http/request.hpp>
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/foreach.hpp>
#include <ostream>
#include <istream>
#include <string>
#include <stdexcept>
#include <map>

namespace boost { namespace network { namespace http {

    template <class tag, unsigned version_major, unsigned version_minor>
    class basic_client {

    private:
        boost::asio::io_service service_;
        boost::asio::ip::tcp::resolver resolver_;
        bool cache_resolved_;
        bool follow_redirect_;

        typedef std::pair<
            boost::asio::ip::tcp::resolver::iterator,
            boost::asio::ip::tcp::resolver::iterator
        > resolver_iterator_pair;

        typedef typename string_traits<tag>::type string_type;

        typedef std::map<string_type, resolver_iterator_pair> resolved_cache;
        resolved_cache endpoint_cache_;

        resolver_iterator_pair resolve(string_type const & hostname, string_type const & port) {
            if (cache_resolved_) {
                typename resolved_cache::iterator cached_iterator =
                    endpoint_cache_.find(hostname);
                if (cached_iterator == endpoint_cache_.end()) {
                    bool inserted = false;
                    boost::tie(cached_iterator, inserted) =
                        endpoint_cache_.insert(
                                std::make_pair(
                                        hostname,
                                        std::make_pair(
                                                resolver_.resolve(
                                                        boost::asio::ip::tcp::resolver::query(
                                                                hostname,
                                                                port,
                                                                boost::asio::ip::tcp::resolver_query::numeric_service
                                                                )
                                                        )
                                                        , boost::asio::ip::tcp::resolver::iterator()
                                                )
                                        )
                                );
                };
                return cached_iterator->second;
            };

            return std::make_pair(
                    resolver_.resolve(
                            boost::asio::ip::tcp::resolver::query(
                                    hostname,
                                    port,
                                    boost::asio::ip::tcp::resolver_query::numeric_service
                                    )
                            )
                            ,
                            boost::asio::ip::tcp::resolver::iterator()
                    );
        };

        void init_socket(boost::asio::ip::tcp::socket & socket_, string_type const & hostname, string_type const & port) {
            using boost::asio::ip::tcp;

            boost::system::error_code error = boost::asio::error::host_not_found;

            tcp::resolver::iterator endpoint_iterator, end;
            boost::tie(endpoint_iterator, end) = resolve(hostname, port);

            while (error && endpoint_iterator != end) {
                socket_.close();
                socket_.connect(*endpoint_iterator++, error);
            }

            if (error)
                throw boost::system::system_error(error);
        };

        void create_request(boost::asio::streambuf & request_buffer, string_type const & method, basic_request<tag> request_) const {
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

            headers_range<http::request>::type range = headers(request_);
            BOOST_FOREACH(headers_range<http::request>::type::value_type const & header, range) {
                request_stream << header.first << ": " << header.second << "\r\n";
            };

            range = headers(request_)["user-agent"];
            if (begin(range) == end(range))
                request_stream << "User-Agent: cpp-netlib/" << BOOST_NETLIB_VERSION << "\r\n";

            request_stream
                << "Connection: close\r\n\r\n";

            string_type body_ = body(request_);
            if (!body_.empty())
                request_stream << body_;
        };

        void send_request(boost::asio::ip::tcp::socket & socket, string_type const & method, basic_request<tag> const & request_) const {
            boost::asio::streambuf request_buffer;
            create_request(request_buffer, method, request_);
            write(socket, request_buffer);
        };

        void read_status(basic_response<tag> & response_, boost::asio::ip::tcp::socket & socket, boost::asio::streambuf & response_buffer) const {
            boost::asio::read_until(socket, response_buffer, "\r\n");
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
        };

        void read_headers(basic_response<tag> & response_, boost::asio::ip::tcp::socket & socket, boost::asio::streambuf & response_buffer) const {
            boost::asio::read_until(socket, response_buffer, "\r\n\r\n");
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
        };

        void read_body(basic_response<tag> & response_, boost::asio::ip::tcp::socket & socket, boost::asio::streambuf & response_buffer) const {
            typename ostringstream_traits<tag>::type body_stream;

            if (response_buffer.size() > 0)
                body_stream << &response_buffer;

            boost::system::error_code error;
            while (boost::asio::read(socket, response_buffer, boost::asio::transfer_at_least(1), error))
                body_stream << &response_buffer;

            if (error != boost::asio::error::eof)
                throw boost::system::system_error(error);

            response_ << body(body_stream.str());
        };

        response const sync_request_skeleton(basic_request<tag> const & request_, string_type method, bool get_body) {
            using boost::asio::ip::tcp;

            basic_request<tag> request_copy(request_);
            basic_response<tag> response_;
            do {
                tcp::socket socket(service_);
                init_socket(socket, request_copy.host(), boost::lexical_cast<string_type>(request_copy.port()));
                send_request(socket, method, request_copy);

                response_ = basic_response<tag>();
                response_ << source(request_copy.host());

                boost::asio::streambuf response_buffer;
                read_status(response_, socket, response_buffer);
                read_headers(response_, socket, response_buffer);
                if (get_body)
                    read_body(response_, socket, response_buffer);

                if (follow_redirect_) {
                    uint16_t status = response_.status();
                    if (status >= 300 && status <= 307) {
                        headers_range<http::request>::type location_range = headers(response_)["Location"];
                        range_iterator<headers_range<http::request>::type>::type location_header = begin(location_range);
                        if (location_header != end(location_range)) {
                            request_copy.uri(location_header->second);
                        } else throw std::runtime_error("Location header not defined in redirect response.");
                    } else break;
                } else break;
            } while(true);

            return response_;
        };

    public:

        struct cache_resolved_type { };

        static cache_resolved_type cache_resolved() {
            return cache_resolved_type();
        };

        struct follow_redirect_type { };

        static follow_redirect_type follow_redirects() {
            return follow_redirect_type();
        };

        static follow_redirect_type follow_redirect() {
            return follow_redirect_type();
        };

        basic_client()
        : service_(), resolver_(service_), cache_resolved_(false), follow_redirect_(false)
        {};

        explicit basic_client(cache_resolved_type (*)())
        : service_(), resolver_(service_), cache_resolved_(true), follow_redirect_(false)
        {};

        explicit basic_client(follow_redirect_type (*)())
        : service_(), resolver_(service_), cache_resolved_(false), follow_redirect_(true)
        {};

        basic_client(cache_resolved_type (*)(), follow_redirect_type (*)())
        : service_(), resolver_(service_), cache_resolved_(true), follow_redirect_(true)
        {};

        void clear_resolved_cache() {
            endpoint_cache_.clear();
        }

        response const head (basic_request<tag> const & request_) {
            return sync_request_skeleton(request_, "HEAD", false);
        };

        response const get (basic_request<tag> const & request_) {
            return sync_request_skeleton(request_, "GET", true);
        };

        response const post (basic_request<tag> const & request_) {
            return sync_request_skeleton(request_, "POST", true);
        };

        response const post (basic_request<tag> const & request_, string_type const & content_type, string_type const & body_) {
            basic_request<tag> request_copy = request_;
            request_copy << body(body_)
                << header("Content-Type", content_type)
                << header("Content-Length", boost::lexical_cast<string_type>(body_.size()));
            return post(request_copy);
        };

        response const post (basic_request<tag> const & request_, string_type const & body_) {
            return post(request_, "x-application/octet-stream", body_);
        };

        response const put (basic_request<tag> const & request_) {
            return sync_request_skeleton(request_, "PUT", true);
        };

        response const put (basic_request<tag> const & request_, string_type const & body_) {
            return put(request_, "x-application/octet-stream", body_);
        };

        response const put (basic_request<tag> const & request_, string_type const & content_type, string_type const & body_) {
            basic_request<tag> request_copy = request_;
            request_copy << body(body_)
                << header("Content-Type", content_type)
                << header("Content-Length", boost::lexical_cast<string_type>(body_.size()));
            return put(request_copy);
        };

        response const delete_ (basic_request<tag> const & request_) {
            return sync_request_skeleton(request_, "DELETE", true);
        };

    };

    typedef basic_client<http::message_tag, 1, 0> client;

} // namespace http

} // namespace network

} // namespace boost

#endif // __NETWORK_PROTOCOL_HTTP_CLIENT_20070908_1_HPP__
