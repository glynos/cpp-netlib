#ifndef BOOST_NETWORK_PROTOCOL_HTTP_IMPL_HTTPS_SYNC_CONNECTION_HTTP_20100601
#define BOOST_NETWORK_PROTOCOL_HTTP_IMPL_HTTPS_SYNC_CONNECTION_HTTP_20100601

// Copyright 2010 (C) Dean Michael Berris
// Copyright 2010 (C) Sinefunc, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/asio/ssl.hpp>

namespace boost { namespace network { namespace http { namespace impl {

    template <class Tag, unsigned version_major, unsigned version_minor>
    struct sync_connection_base_impl;

    template <class Tag, unsigned version_major, unsigned version_minor>
    struct sync_connection_base;

    template <class Tag, unsigned version_major, unsigned version_minor>
    struct https_sync_connection : public virtual sync_connection_base<Tag,version_major,version_minor>, sync_connection_base_impl<Tag, version_major, version_minor> {
        typedef typename resolver_policy<Tag>::type resolver_base;
        typedef typename resolver_base::resolver_type resolver_type;
        typedef typename string<Tag>::type string_type;
        typedef function<typename resolver_base::resolver_iterator_pair(resolver_type&, string_type const &, string_type const &)> resolver_function_type;
        typedef sync_connection_base_impl<Tag,version_major,version_minor> connection_base;
        
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
            linearize(request_, method, version_major, version_minor,
                std::ostreambuf_iterator<typename char_<Tag>::type>(&request_buffer));
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
            typename headers_range<basic_response<Tag> >::type connection_range =
                headers(response_)["Connection"];
            if (version_major == 1 && version_minor == 1 && !empty(connection_range) && boost::iequals(boost::begin(connection_range)->second, "close")) {
                close_socket();
            } else if (version_major == 1 && version_minor == 0) {
                close_socket();
            }
        }

        bool is_open() { 
            return socket_.lowest_layer().is_open(); 
        }

        void close_socket() { 
            boost::system::error_code ignored;
            socket_.lowest_layer().shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored);
            socket_.lowest_layer().close(ignored);
        }

        ~https_sync_connection() {
            close_socket();
        }

        private:
        resolver_type & resolver_;
        resolver_function_type resolve_;
        boost::asio::ssl::context context_;
        boost::asio::ssl::stream<boost::asio::ip::tcp::socket> socket_;
        
    };

} // namespace impl

} // namespace http

} // namespace network

} // namespace boost

#endif // BOOST_NETWORK_PROTOCOL_HTTP_IMPL_HTTPS_SYNC_CONNECTION_HTTP_20100601
