#ifndef BOOST_NETWORK_PROTOCOL_HTTP_IMPL_SYNC_CONNECTION_BASE_20091217
#define BOOST_NETWORK_PROTOCOL_HTTP_IMPL_SYNC_CONNECTION_BASE_20091217

//          Copyright Dean Michael Berris 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/protocol/http/traits/resolver_policy.hpp>
#include <boost/network/protocol/http/detail/connection_helper.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/tuple/tuple.hpp>

namespace boost { namespace network { namespace http { namespace impl {
    template <class Tag, unsigned version_major, unsigned version_minor>
    struct connection_base_impl {
        protected:
        typedef typename resolver_policy<Tag>::type resolver_base;
        typedef typename resolver_base::resolver_type resolver_type;
        typedef typename string<Tag>::type string_type;
        typedef function<typename resolver_base::resolver_iterator_pair(resolver_type&, string_type const &, string_type const &)> resolver_function_type;

        template <class Socket>
        void init_socket(Socket & socket_, resolver_type & resolver_, string_type const & hostname, string_type const & port, resolver_function_type resolve_) {
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

        template <class Socket>
        void read_status(Socket & socket_, basic_response<Tag> & response_, boost::asio::streambuf & response_buffer) {
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

        template <class Socket>
        void read_headers(Socket & socket_, basic_response<Tag> & response_, boost::asio::streambuf & response_buffer) {
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

        template <class Socket>
        void send_request_impl(Socket & socket_, string_type const & method, boost::asio::streambuf & request_buffer) {
            write(socket_, request_buffer);
        }

        template <class Socket>
        void read_body(Socket & socket_, basic_response<Tag> & response_, boost::asio::streambuf & response_buffer) {
            typename ostringstream<Tag>::type body_stream;

            if (response_buffer.size() > 0)
                body_stream << &response_buffer;

            boost::system::error_code error;
            if (!connection_keepalive<Tag>::value) {
                while (boost::asio::read(socket_, response_buffer, boost::asio::transfer_at_least(1), error)) {
                    body_stream << &response_buffer;
                }
            } else {
                // look for the content-length header
                typename headers_range<basic_response<Tag> >::type content_length_range =
                    headers(response_)["Content-Length"];
                if (empty(content_length_range))
                    throw std::runtime_error("Unsupported response, missing 'Content-Length' header.");
                size_t length = lexical_cast<size_t>(begin(content_length_range)->second);
                std::cerr << "Before reading body of size " << length << "...\n";
                size_t bytes_read = 0;
                while ((bytes_read = boost::asio::read(socket_, response_buffer, boost::asio::transfer_at_least(1), error))) {
                    body_stream << &response_buffer;
                    length -= bytes_read;
                    if ((length <= 0) or error)
                        break;
                }
                std::cerr << "After reading body of size " << length << "...\n";
            }

            if (error != boost::asio::error::eof)
                throw boost::system::system_error(error);

            response_ << body(body_stream.str());
        }


    };

    template <class Tag, unsigned version_major, unsigned version_minor>
    struct sync_connection_base;

    template <class Tag, unsigned version_major, unsigned version_minor>
    struct https_sync_connection : public virtual sync_connection_base<Tag,version_major,version_minor>, detail::connection_helper<Tag, version_major, version_minor>, connection_base_impl<Tag, version_major, version_minor> {
        typedef typename resolver_policy<Tag>::type resolver_base;
        typedef typename resolver_base::resolver_type resolver_type;
        typedef typename string<Tag>::type string_type;
        typedef function<typename resolver_base::resolver_iterator_pair(resolver_type&, string_type const &, string_type const &)> resolver_function_type;
        typedef connection_base_impl<Tag,version_major,version_minor> connection_base;
        
        https_sync_connection(resolver_type & resolver, resolver_function_type resolve, optional<string_type> const & certificate_filename = optional<string_type>())
        : connection_base(), resolver_(resolver), resolve_(resolve), context_(resolver.io_service(), boost::asio::ssl::context::sslv23_client), socket_(resolver.io_service(), context_) {
            if (certificate_filename) {
                context_.set_verify_mode(boost::asio::ssl::context::verify_peer);
                context_.load_verify_file(*certificate_filename);
            } else {
                context_.set_verify_mode(boost::asio::ssl::context::verify_none);
            }
        }

        void init_socket(string_type const & hostname, string_type const & port) {
            connection_base::init_socket(socket_.lowest_layer(), resolver_, hostname, port, resolve_);
            socket_.handshake(boost::asio::ssl::stream_base::client);
        }

        void send_request_impl(string_type const & method, basic_request<Tag> const & request_) {
            boost::asio::streambuf request_buffer;
            create_request(request_buffer, method, request_);
            connection_base::send_request_impl(socket_, method, request_buffer);
        }

        void read_status(basic_response<Tag> & response_, boost::asio::streambuf & response_buffer) {
            connection_base::read_status(socket_, response_, response_buffer);
        }

        void read_headers(basic_response<Tag> & response_, boost::asio::streambuf & response_buffer) {
            connection_base::read_headers(socket_, response_, response_buffer);
        }

        void read_body(basic_response<Tag> & response_, boost::asio::streambuf & response_buffer) {
            connection_base::read_body(socket_, response_, response_buffer);    
        }

        bool is_open() { 
            return socket_.lowest_layer().is_open(); 
        }

        void close_socket() { 
            if (is_open()) {
                socket_.lowest_layer().close();
            } 
        }

        ~https_sync_connection() {}

        private:
        resolver_type & resolver_;
        resolver_function_type resolve_;
        boost::asio::ssl::context context_;
        boost::asio::ssl::stream<boost::asio::ip::tcp::socket> socket_;
        
    };

    template <class Tag, unsigned version_major, unsigned version_minor>
    struct http_sync_connection : public virtual sync_connection_base<Tag, version_major, version_minor>, detail::connection_helper<Tag, version_major, version_minor>, connection_base_impl<Tag, version_major, version_minor> {
        typedef typename resolver_policy<Tag>::type resolver_base;
        typedef typename resolver_base::resolver_type resolver_type;
        typedef typename string<Tag>::type string_type;
        typedef function<typename resolver_base::resolver_iterator_pair(resolver_type&, string_type const &, string_type const &)> resolver_function_type;
        typedef connection_base_impl<Tag,version_major,version_minor> connection_base;

        http_sync_connection(resolver_type & resolver, resolver_function_type resolve)
        : connection_base(), resolver_(resolver), resolve_(resolve), socket_(resolver.io_service()) { }

        ~http_sync_connection() {}

        void init_socket(string_type const & hostname, string_type const & port) {
            connection_base::init_socket(socket_, resolver_, hostname, port, resolve_);
        }

        void send_request_impl(string_type const & method, basic_request<Tag> const & request_) {
            boost::asio::streambuf request_buffer;
            create_request(request_buffer, method, request_);
            connection_base::send_request_impl(socket_, method, request_buffer);
        }

        void read_status(basic_response<Tag> & response_, boost::asio::streambuf & response_buffer) {
            connection_base::read_status(socket_, response_, response_buffer);
        }

        void read_headers(basic_response<Tag> & response, boost::asio::streambuf & response_buffer) {
            connection_base::read_headers(socket_, response, response_buffer);
        }

        void read_body(basic_response<Tag> & response_, boost::asio::streambuf & response_buffer) {
            connection_base::read_body(socket_, response_, response_buffer);
        }

        bool is_open() { return socket_.is_open(); }

        void close_socket() { if (is_open()) { socket_.close(); } }

        private:

        resolver_type & resolver_;
        resolver_function_type resolve_;
        boost::asio::ip::tcp::socket socket_;

    };

    template <class Tag, unsigned version_major, unsigned version_minor>
    struct sync_connection_base {
        typedef typename resolver_policy<Tag>::type resolver_base;
        typedef typename resolver_base::resolver_type resolver_type;
        typedef typename string<Tag>::type string_type;
        typedef function<typename resolver_base::resolver_iterator_pair(resolver_type&, string_type const &, string_type const &)> resolver_function_type;

        static sync_connection_base<Tag,version_major,version_minor> * new_connection(resolver_type & resolver, resolver_function_type resolve, bool https) {
            if (https) {
                return dynamic_cast<sync_connection_base<Tag,version_major,version_minor>*>(new https_sync_connection<Tag,version_major,version_minor>(resolver, resolve));
            }
            return dynamic_cast<sync_connection_base<Tag,version_major,version_minor>*>(new http_sync_connection<Tag,version_major,version_minor>(resolver, resolve));
        }

        virtual void init_socket(string_type const & hostname, string_type const & port) = 0;
        virtual void send_request_impl(string_type const & method, basic_request<Tag> const & request_) = 0;
        virtual void read_status(basic_response<Tag> & response_, boost::asio::streambuf & response_buffer) = 0;
        virtual void read_headers(basic_response<Tag> & response_, boost::asio::streambuf & response_buffer) = 0;
        virtual void read_body(basic_response<Tag> & response_, boost::asio::streambuf & response_buffer) = 0;
        virtual bool is_open() = 0;
        virtual void close_socket() = 0;
        virtual ~sync_connection_base() {}
        protected:
        sync_connection_base() {}
    };

} // namespace impl

} // namespace http

} // namespace network

} // namespace boost

#endif // BOOST_NETWORK_PROTOCOL_HTTP_IMPL_SYNC_CONNECTION_BASE_20091217

