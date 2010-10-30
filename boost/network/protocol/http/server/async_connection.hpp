#ifndef BOOST_NETWORK_PROTOCOL_HTTP_SERVER_CONNECTION_HPP_20101027
#define BOOST_NETWORK_PROTOCOL_HTTP_SERVER_CONNECTION_HPP_20101027

// Copyright 2010 Dean Michael Berris.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/throw_exception.hpp>
#include <boost/scope_exit.hpp>
#include <boost/network/protocol/http/algorithms/linearize.hpp>
#include <boost/network/utils/thread_pool.hpp>
#include <boost/range/algorithm/transform.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/asio/strand.hpp>
#include <boost/asio/buffer.hpp>
#include <iterator>

#ifndef BOOST_NETWORK_HTTP_SERVER_CONNECTION_HEADER_BUFFER_MAX_SIZE
/** Here we define a page's worth of header connection buffer data.
 *  This can be tuned to reduce the memory cost of connections, but this 
 *  default size is set to be friendly to typical service applications.
 *  This is the maximum size though and Boost.Asio's internal representation
 *  of a streambuf would make appropriate decisions on how big a buffer
 *  is to begin with.
 */
#define BOOST_NETWORK_HTTP_SERVER_CONNECTION_HEADER_BUFFER_MAX_SIZE 4096
#endif /* BOOST_NETWORK_HTTP_SERVER_CONNECTION_HEADER_BUFFER_MAX_SIZE */

namespace boost { namespace network { namespace http {

    template <class Tag, class Handler>
    struct async_connection : boost::enable_shared_from_this<async_connection<Tag,Handler> > {
        enum status_t {
            ok = 200
            , created = 201
            , accepted = 202
            , no_content = 204
            , multiple_choices = 300
            , moved_permanently = 301
            , moved_temporarily = 302
            , not_modified = 304
            , bad_request = 400
            , unauthorized = 401
            , forbidden = 403
            , not_found = 404
            , not_supported = 405
            , not_acceptable = 406
            , internal_server_error = 500
            , not_implemented = 501
            , bad_gateway = 502
            , service_unavailable = 503
        };

        async_connection(
            asio::io_service & io_service
            , Handler & handler
            , utils::thread_pool & thread_pool
            )
        : socket_(io_service)
        , strand(io_service)
        , handler(handler)
        , thread_pool_(thread_pool)
        , headers_already_sent(false)
        , headers_buffer(BOOST_NETWORK_HTTP_SERVER_CONNECTION_HEADER_BUFFER_MAX_SIZE)
        {}

        /** Function: template <class Range> set_headers(Range headers)
         *  Precondition: headers have not been sent yet
         *  Postcondition: headers have been linearized to a buffer.
         *  Throws: std::logic_error in case the headers have already been sent. 
         *
         *  A call to set_headers takes a Range where each element models the
         *  Header concept. This Range will be linearized onto a buffer, which is
         *  then sent as soon as the first call to `write` or `flush` commences.
         */
        template <class Range>
        void set_headers(Range headers) {
            if (headers_already_sent)
                boost::throw_exception(std::logic_error("Headers have already been sent."));

            bool commit = false;
            BOOST_SCOPE_EXIT_TPL((&commit)(&headers_already_sent)) {
                if (!commit) headers_already_sent = false;
            } BOOST_SCOPE_EXIT_END

            typedef constants<Tag> consts;
            headers_buffer.consume(headers_buffer.size());
            std::ostream stream(&headers_buffer);
            if (!boost::empty(headers)) {
                typedef typename Range::const_iterator iterator;
                typedef typename Range::value_type value_type;
                typedef typename string<Tag>::type string_type;
                boost::transform(headers, 
                    std::ostream_iterator<string_type>(stream),
                    linearize<Tag, value_type>());
            } else {
                stream << consts::crlf();
            }
            stream << consts::crlf();
            commit = true;
        }

        void set_status(status_t new_status) {
            status = new_status;
        }

        template <class Range>
        void write(Range) {
            if (!headers_already_sent) {
                // TODO write out the headers that are already
                // linearized to the headers_buffer.
            }
            // linearize the range into a shared array
            // schedule a stranded asynchronous write
        }

        asio::ip::tcp::socket & socket()    { return socket_;               }
        utils::thread_pool & thread_pool()  { return thread_pool_;          }

    private:
        asio::ip::tcp::socket socket_;
        asio::io_service::strand strand;
        Handler & handler;
        utils::thread_pool & thread_pool_;
        bool headers_already_sent;
        asio::streambuf headers_buffer;

        typedef boost::array<char, BOOST_NETWORK_HTTP_SERVER_CONNECTION_BUFFER_SIZE> 
            buffer_type;
        buffer_type read_buffer_;
        boost::uint16_t status;

        template <class, class> friend struct async_server_base;

        enum state_t {
            method, uri, version, headers
        };

        void start() {
            socket_.async_read_some(
                asio::buffer(read_buffer_)
                , strand.wrap(
                    boost::bind(
                        &async_connection<Tag,Handler>::handle_read_data,
                        async_connection<Tag,Handler>::shared_from_this(),
                        method,
                        boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred
                        )
                    )
                );
        }

        void handle_read_data(state_t, boost::system::error_code const & ec, std::size_t bytes_transferred) {
            // FIXME -- damn all that work got wiped out because Jeni tripped on the power. :(
        }

    };
    
} /* http */
    
} /* network */
    
} /* boost */

#endif /* BOOST_NETWORK_PROTOCOL_HTTP_SERVER_CONNECTION_HPP_20101027 */

