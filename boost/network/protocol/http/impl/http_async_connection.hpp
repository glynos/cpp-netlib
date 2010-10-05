#ifndef BOOST_NETWORK_PROTOCOL_HTTP_IMPL_HTTP_ASYNC_CONNECTION_HPP_20100601
#define BOOST_NETWORK_PROTOCOL_HTTP_IMPL_HTTP_ASYNC_CONNECTION_HPP_20100601

// Copyright 2010 (C) Dean Michael Berris
// Copyright 2010 (C) Sinefunc, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/version.hpp>
#include <boost/thread/future.hpp>
#include <boost/throw_exception.hpp>
#include <boost/cstdint.hpp>
#include <boost/range/algorithm/transform.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/network/constants.hpp>
#include <boost/network/traits/ostream_iterator.hpp>
#include <boost/network/traits/istream.hpp>
#include <boost/logic/tribool.hpp>
#include <boost/network/protocol/http/parser/incremental.hpp>
#include <boost/network/protocol/http/message/wrappers/uri.hpp>
#include <boost/array.hpp>
#include <iterator>

namespace boost { namespace network { namespace http { namespace impl {

    template <class Tag, unsigned version_major, unsigned version_minor>
    struct async_connection_base;

    template <class Tag, unsigned version_major, unsigned version_minor>
    struct http_async_connection
        : async_connection_base<Tag,version_major,version_minor>,
        boost::enable_shared_from_this<http_async_connection<Tag,version_major,version_minor> >
    {
            typedef async_connection_base<Tag,version_major,version_minor> base;
            typedef typename base::resolver_type resolver_type;
            typedef typename base::resolver_base::resolver_iterator resolver_iterator;
            typedef typename base::resolver_base::resolver_iterator_pair resolver_iterator_pair;
            typedef typename base::response response;
            typedef typename base::string_type string_type;
            typedef typename base::request request;
            typedef typename base::resolver_base::resolve_function resolve_function;

            http_async_connection(
                boost::shared_ptr<resolver_type> resolver,
                resolve_function resolve, 
                bool follow_redirect 
                ) : resolver_(resolver),
                resolve_(resolve),
                follow_redirect_(follow_redirect),
                request_strand_(new boost::asio::io_service::strand(resolver->get_io_service()))
            {}

            struct to_http_headers {
                typedef typename string<Tag>::type string_type;
                template <class U>
                string_type const operator() (U const & pair) const {
                    typedef typename ostringstream<Tag>::type ostringstream_type;
                    typedef constants<Tag> constants;
                    ostringstream_type header_line;
                    header_line << pair.first
                        << constants::colon()
                        << constants::space()
                        << pair.second
                        << constants::crlf();
                    return header_line.str();
                }
            };

            virtual response start(request const & request, string_type const & method, bool get_body) {
                response temp;
                boost::shared_future<string_type> source_future(source_promise.get_future());
                source(temp, source_future);
                boost::shared_future<string_type> destination_future(destination_promise.get_future());
                destination(temp, destination_future);
                boost::shared_future<typename headers_container<Tag>::type> headers_future(headers_promise.get_future());
                headers(temp, headers_future);
                boost::shared_future<string_type> body_future(body_promise.get_future());
                body(temp, body_future);
                boost::shared_future<string_type> version_future(version_promise.get_future());
                version(temp, version_future);
                boost::shared_future<boost::uint16_t> status_future(status_promise.get_future());
                status(temp, status_future);
                boost::shared_future<string_type> status_message_future(status_message_promise.get_future());
                status_message(temp, status_message_future);

                if (!get_body) body_promise.set_value(string_type());

                typename ostringstream<Tag>::type command_stream;
                string_type path_str;
                path_str = path(request);
                typedef constants<Tag> constants;
                command_stream 
                    << method << constants::space()
                    << path_str << constants::space()
                    << constants::http_slash() << version_major
                    << constants::dot() << version_minor
                    << constants::crlf();
                
                typedef typename headers_range<typename base::request>::type headers_range_type;
                headers_range_type headers_ = headers(request);
                boost::range::transform(
                    headers_,
                    typename ostream_iterator<Tag, string_type>::type (command_stream),
                    to_http_headers());

                if (boost::empty(headers(request)[constants::host()])) {
                    string_type host_str = host(request);
                    command_stream
                        << constants::host() << constants::colon() << constants::space() << host_str << constants::crlf();
                }

                if (boost::empty(headers(request)[constants::accept()])) {
                    command_stream
                        << constants::accept() << constants::colon() << constants::space() << constants::default_accept_mime() << constants::crlf();
                }

                if (version_major == 1u && version_minor == 1u && boost::empty(headers(request)[constants::accept_encoding()])) {
                    command_stream
                        << constants::accept_encoding() << constants::colon() << constants::space() << constants::default_accept_encoding() << constants::crlf();
                }

                if (boost::empty(headers(request)[constants::user_agent()])) {
                    command_stream
                        << constants::user_agent() << constants::colon() << constants::space() << constants::cpp_netlib_slash() << BOOST_NETLIB_VERSION << constants::crlf();
                }

                command_stream << constants::crlf();

                command_string_ = command_stream.str();

                this->method = method;

                boost::uint16_t port_ = port(request);

                resolve_(resolver_, host(request), port_,
                    request_strand_->wrap(
                        boost::bind(
                        &http_async_connection<Tag,version_major,version_minor>::handle_resolved,
                        http_async_connection<Tag,version_major,version_minor>::shared_from_this(),
                        port_,
                        _1, _2)));
                return temp;
            }

    private:
        void handle_resolved(boost::uint16_t port, boost::system::error_code const & ec, resolver_iterator_pair endpoint_range) {
            resolver_iterator iter = boost::begin(endpoint_range);
            if (!ec && !boost::empty(endpoint_range)) {
                boost::asio::ip::tcp::endpoint endpoint(
                    iter->endpoint().address(),
                    port
                    );
                socket_.reset(new boost::asio::ip::tcp::socket(
                    resolver_->get_io_service()));
                socket_->async_connect(
                    endpoint,
                    request_strand_->wrap(
                        boost::bind(
                            &http_async_connection<Tag,version_major,version_minor>::handle_connected,
                            http_async_connection<Tag,version_major,version_minor>::shared_from_this(),
                            port, std::make_pair(++iter, resolver_iterator()),
                            boost::asio::placeholders::error
                            )));
            } else {
                boost::system::system_error error(ec ? ec : boost::asio::error::host_not_found);
                version_promise.set_exception(boost::copy_exception(error));
                status_promise.set_exception(boost::copy_exception(error));
                status_message_promise.set_exception(boost::copy_exception(error));
                headers_promise.set_exception(boost::copy_exception(error));
                source_promise.set_exception(boost::copy_exception(error));
                destination_promise.set_exception(boost::copy_exception(error));
                body_promise.set_exception(boost::copy_exception(error));
            }
        }

        void handle_connected(boost::uint16_t port, resolver_iterator_pair endpoint_range, boost::system::error_code const & ec) {
            if (!ec) {
                boost::asio::async_write(*socket_, boost::asio::buffer(command_string_.data(), command_string_.size()),
                    request_strand_->wrap(
                        boost::bind(
                            &http_async_connection<Tag,version_major,version_minor>::handle_sent_request,
                            http_async_connection<Tag,version_major,version_minor>::shared_from_this(),
                            boost::asio::placeholders::error,
                            boost::asio::placeholders::bytes_transferred
                            )));
            } else {
                if (!boost::empty(endpoint_range)) {
                    resolver_iterator iter = boost::begin(endpoint_range);
                    boost::asio::ip::tcp::endpoint endpoint(
                        iter->endpoint().address(),
                        port
                        );
                    socket_.reset(new boost::asio::ip::tcp::socket(
                        resolver_->get_io_service()));
                    socket_->async_connect(
                        endpoint,
                        request_strand_->wrap(
                            boost::bind(
                                &http_async_connection<Tag,version_major,version_minor>::handle_connected,
                                http_async_connection<Tag,version_major,version_minor>::shared_from_this(),
                                port, std::make_pair(++iter, resolver_iterator()),
                                boost::asio::placeholders::error
                                )));
                } else {
                    boost::system::system_error error(
                        ec ? ec : boost::asio::error::host_not_found
                        );
                    version_promise.set_exception(boost::copy_exception(error));
                    status_promise.set_exception(boost::copy_exception(error));
                    status_message_promise.set_exception(boost::copy_exception(error));
                    headers_promise.set_exception(boost::copy_exception(error));
                    source_promise.set_exception(boost::copy_exception(error));
                    destination_promise.set_exception(boost::copy_exception(error));
                    body_promise.set_exception(boost::copy_exception(error));
                }
            }
        }

        void handle_sent_request(boost::system::error_code const & ec, std::size_t bytes_transferred) {
            if (!ec) {
                boost::asio::async_read(
                    *socket_,
                    boost::asio::mutable_buffers_1(part.c_array(), part.size()),
                    request_strand_->wrap(
                        boost::bind(
                            &http_async_connection<Tag,version_major,version_minor>::handle_received_version,
                            http_async_connection<Tag,version_major,version_minor>::shared_from_this(),
                            boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)));
            } else {
                boost::system::system_error error(
                    ec ? ec : boost::asio::error::host_not_found
                    );
                version_promise.set_exception(boost::copy_exception(error));
                status_promise.set_exception(boost::copy_exception(error));
                status_message_promise.set_exception(boost::copy_exception(error));
                headers_promise.set_exception(boost::copy_exception(error));
                source_promise.set_exception(boost::copy_exception(error));
                destination_promise.set_exception(boost::copy_exception(error));
                body_promise.set_exception(boost::copy_exception(error));
            }
        }

        void parse_version() {
            logic::tribool parsed_ok;
            typename boost::iterator_range<typename buffer_type::const_iterator> result_range;
            fusion::tie(parsed_ok, result_range) = response_parser_.parse_until(
                response_parser_type::http_version_done,
                part);
            if (parsed_ok == true) {
                string_type version;
                std::swap(version, partial_parsed);
                version.append(boost::begin(result_range), boost::end(result_range));
                algorithm::trim(version);
                version_promise.set_value(version);
                part_begin = boost::end(result_range);
                this->parse_status();
            } else if (parsed_ok == false) {
                std::runtime_error error("Invalid Version Part.");
                version_promise.set_exception(boost::copy_exception(error));
                status_promise.set_exception(boost::copy_exception(error));
                status_message_promise.set_exception(boost::copy_exception(error));
                headers_promise.set_exception(boost::copy_exception(error));
                source_promise.set_exception(boost::copy_exception(error));
                destination_promise.set_exception(boost::copy_exception(error));
                body_promise.set_exception(boost::copy_exception(error));
            } else {
                partial_parsed.append(
                    boost::begin(result_range),
                    boost::end(result_range)
                    );
                part_begin = part.begin();
                boost::asio::async_read(
                    *socket_,
                    boost::asio::mutable_buffers_1(part.c_array(), part.size()),
                    request_strand_->wrap(
                        boost::bind(
                            &http_async_connection<Tag,version_major,version_minor>::handle_received_version,
                            http_async_connection<Tag,version_major,version_minor>::shared_from_this(),
                            boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)));
            }
        }

        void handle_received_version(boost::system::error_code const & ec, std::size_t bytes_transferred) {
            if (!ec || ec == boost::asio::error::eof) {
                this->parse_version();
            } else {
                boost::system::system_error error(ec);
                version_promise.set_exception(boost::copy_exception(error));
                status_promise.set_exception(boost::copy_exception(error));
                status_message_promise.set_exception(boost::copy_exception(error));
                headers_promise.set_exception(boost::copy_exception(error));
                source_promise.set_exception(boost::copy_exception(error));
                destination_promise.set_exception(boost::copy_exception(error));
                body_promise.set_exception(boost::copy_exception(error));
            }
        }

        void parse_status() {
            logic::tribool parsed_ok;
            typename buffer_type::const_iterator part_end = part.end();
            typename boost::iterator_range<typename buffer_type::const_iterator> result_range,
                input_range = boost::make_iterator_range(part_begin, part_end);
            fusion::tie(parsed_ok, result_range) = response_parser_.parse_until(
                response_parser_type::http_status_done,
                input_range);
            if (parsed_ok == true) {
                string_type status;
                std::swap(status, partial_parsed);
                status.append(boost::begin(result_range), boost::end(result_range));
                trim(status);
                boost::uint16_t status_int = lexical_cast<boost::uint16_t>(status);
                status_promise.set_value(status_int);
                part_begin = boost::end(result_range);
                this->parse_status_message();
            } else if (parsed_ok == false) {
                std::runtime_error error("Invalid status part.");
                status_promise.set_exception(boost::copy_exception(error));
                status_message_promise.set_exception(boost::copy_exception(error));
                headers_promise.set_exception(boost::copy_exception(error));
                source_promise.set_exception(boost::copy_exception(error));
                destination_promise.set_exception(boost::copy_exception(error));
                body_promise.set_exception(boost::copy_exception(error));
            } else {
                partial_parsed.append(
                    boost::begin(result_range), 
                    boost::end(result_range)
                    );
                part_begin = part.begin();
                boost::asio::async_read(*socket_, 
                    boost::asio::mutable_buffers_1(part.c_array(), part.size()),
                    request_strand_->wrap(
                        boost::bind(
                            &http_async_connection<Tag,version_major,version_minor>::handle_received_status,
                            http_async_connection<Tag,version_major,version_minor>::shared_from_this(),
                            boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)));
            }
        }

        void handle_received_status(boost::system::error_code const & ec, size_t bytes_transferred) {
            if (!ec || ec == boost::asio::error::eof) {
                this->parse_status();
            } else {
                boost::system::system_error error(ec);
                status_promise.set_exception(boost::copy_exception(error));
                status_message_promise.set_exception(boost::copy_exception(error));
                headers_promise.set_exception(boost::copy_exception(error));
                source_promise.set_exception(boost::copy_exception(error));
                destination_promise.set_exception(boost::copy_exception(error));
                body_promise.set_exception(boost::copy_exception(error));
            }
        }

        void parse_status_message() {
            logic::tribool parsed_ok;
            typename buffer_type::const_iterator part_end = part.end();
            typename boost::iterator_range<typename buffer_type::const_iterator> result_range,
                input_range = boost::make_iterator_range(part_begin, part_end);
            fusion::tie(parsed_ok, result_range) = response_parser_.parse_until(
                response_parser_type::http_status_message_done,
                input_range);
            if (parsed_ok == true) {
                string_type status_message;
                std::swap(status_message, partial_parsed);
                status_message.append(boost::begin(result_range), boost::end(result_range));
                algorithm::trim(status_message);
                status_message_promise.set_value(status_message);
                part_begin = boost::end(result_range);
                this->parse_headers();
            } else if (parsed_ok == false) {
                std::runtime_error error("Invalid status message part.");
                status_message_promise.set_exception(boost::copy_exception(error));
                headers_promise.set_exception(boost::copy_exception(error));
                source_promise.set_exception(boost::copy_exception(error));
                destination_promise.set_exception(boost::copy_exception(error));
                body_promise.set_exception(boost::copy_exception(error));
            } else {
                partial_parsed.append(
                    boost::begin(result_range), 
                    boost::end(result_range));
                part_begin = part.begin();
                boost::asio::async_read(
                    *socket_,
                    boost::asio::mutable_buffers_1(part.c_array(), part.size()),
                    request_strand_->wrap(
                        boost::bind(
                            &http_async_connection<Tag,version_major,version_minor>::handle_received_status_message,
                            http_async_connection<Tag,version_major,version_minor>::shared_from_this(),
                            boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)));
            }
        }

        void handle_received_status_message(boost::system::error_code const & ec, size_t bytes_transferred) {
            if (!ec || ec == boost::asio::error::eof) {
                this->parse_status_message();
            } else {
                boost::system::system_error error(ec);
                status_message_promise.set_exception(boost::copy_exception(error));
                headers_promise.set_exception(boost::copy_exception(error));
                source_promise.set_exception(boost::copy_exception(error));
                destination_promise.set_exception(boost::copy_exception(error));
                body_promise.set_exception(boost::copy_exception(error));
            }
        }

        void parse_headers_real(string_type & headers_part) {
            typename boost::iterator_range<typename string_type::const_iterator> 
                input_range = boost::make_iterator_range(headers_part)
                , result_range;
            logic::tribool parsed_ok;
            response_parser_type headers_parser(response_parser_type::http_header_line_done);
            typename headers_container<Tag>::type headers;
            std::pair<string_type,string_type> header_pair;
            while (!boost::empty(input_range)) {
                fusion::tie(parsed_ok, result_range) = headers_parser.parse_until(
                    response_parser_type::http_header_colon
                    , input_range);
                if (headers_parser.state() != response_parser_type::http_header_colon) break;
                header_pair.first = string_type(
                    boost::begin(result_range), 
                    boost::end(result_range));
                input_range.advance_begin(boost::distance(result_range));
                fusion::tie(parsed_ok, result_range) = headers_parser.parse_until(
                    response_parser_type::http_header_line_done
                    , input_range);
                header_pair.second = string_type(
                    boost::begin(result_range), 
                    boost::end(result_range));
                input_range.advance_begin(boost::distance(result_range));

                trim(header_pair.first);
                if (header_pair.first.size() > 1) {
                    header_pair.first.erase(
                        header_pair.first.size() - 1
                    );
                }
                trim(header_pair.second);
                headers.insert(header_pair);
            }
            headers_promise.set_value(headers);
        }

        void parse_headers() {
            logic::tribool parsed_ok;
            typename buffer_type::const_iterator part_end = part.end();
            typename boost::iterator_range<typename buffer_type::const_iterator> result_range,
                input_range = boost::make_iterator_range(part_begin, part_end);
            fusion::tie(parsed_ok, result_range) = response_parser_.parse_until(
                response_parser_type::http_headers_done,
                input_range);
            if (parsed_ok == true) {
                string_type headers_string;
                std::swap(headers_string, partial_parsed);
                headers_string.append(boost::begin(result_range), boost::end(result_range));
                part_begin = boost::end(result_range);
                this->parse_headers_real(headers_string);
                this->parse_body(std::distance(boost::end(result_range), part_end));
            } else if (parsed_ok == false) {
                std::runtime_error error("Invalid header part.");
            } else {
                partial_parsed.append(boost::begin(result_range), boost::end(result_range));
                part_begin = part.begin();
                boost::asio::async_read(
                    *socket_,
                    boost::asio::mutable_buffers_1(part.c_array(), part.size()),
                    request_strand_->wrap(
                        boost::bind(
                            &http_async_connection<Tag,version_major,version_minor>::handle_received_headers,
                            http_async_connection<Tag,version_major,version_minor>::shared_from_this(),
                            boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)));
            }
        }

        void handle_received_headers(boost::system::error_code const & ec, size_t bytes_transferred) {
            if (!ec || ec == boost::asio::error::eof) {
                this->parse_headers();
            } else {
                boost::system::system_error error(ec);
                headers_promise.set_exception(boost::copy_exception(error));
                source_promise.set_exception(boost::copy_exception(error));
                destination_promise.set_exception(boost::copy_exception(error));
                body_promise.set_exception(boost::copy_exception(error));
            }
        }

        void parse_body(size_t bytes) {
            partial_parsed.append(part_begin, bytes);
            part_begin = part.begin();
            boost::asio::async_read(*socket_, boost::asio::mutable_buffers_1(part.c_array(), part.size()),
                request_strand_->wrap(
                    boost::bind(
                        &http_async_connection<Tag,version_major,version_minor>::handle_recieved_body,
                        http_async_connection<Tag,version_major,version_minor>::shared_from_this(),
                        boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)));
        }

        void handle_recieved_body(boost::system::error_code const & ec, size_t bytes_transferred) {
            if (ec == boost::asio::error::eof) {
                string_type body;
                std::swap(body, partial_parsed);
                body.append(
                    part.begin()
                    , bytes_transferred
                    );
                body_promise.set_value(body);
                // TODO set the destination value somewhere!
                destination_promise.set_value("");
                source_promise.set_value("");
                part.assign('\0');
                response_parser_.reset();
            } else if (!ec) {
                this->parse_body(bytes_transferred);
            } else {
                boost::system::system_error error(ec);
                source_promise.set_exception(boost::copy_exception(error));
                destination_promise.set_exception(boost::copy_exception(error));
                body_promise.set_exception(boost::copy_exception(error));
            }
        }

        typedef response_parser<Tag> response_parser_type;
        typedef boost::array<typename char_<Tag>::type, 1024> buffer_type;

        boost::shared_ptr<resolver_type> resolver_;
        resolve_function resolve_;
        bool follow_redirect_;
        boost::shared_ptr<boost::asio::io_service::strand> request_strand_;
        boost::shared_ptr<boost::asio::ip::tcp::socket> socket_;
        boost::promise<string_type> version_promise;
        boost::promise<boost::uint16_t> status_promise;
        boost::promise<string_type> status_message_promise;
        boost::promise<typename headers_container<Tag>::type> headers_promise;
        boost::promise<string_type> source_promise;
        boost::promise<string_type> destination_promise;
        boost::promise<string_type> body_promise;
        string_type command_string_;
        response_parser_type response_parser_;
        buffer_type part;
        typename buffer_type::const_iterator part_begin;
        string_type partial_parsed;
        string_type method;
    };

} // namespace impl

} // namespace http

} // namespace network

} // namespace boost

#endif // BOOST_NETWORK_PROTOCOL_HTTP_IMPL_HTTP_ASYNC_CONNECTION_HPP_20100601
