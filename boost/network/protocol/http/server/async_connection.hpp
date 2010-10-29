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
#include <iterator>

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
        , handler(handler)
        , thread_pool_(thread_pool)
        , headers_already_set(false)
        {}

        /** Function: template <class Range> set_headers(Range headers)
         *  Precondition: headers have not been set yet
         *  Postcondition: headers have been set, and cannot be modified anymore.
         *  Throws: std::logic_error in case the headers have already been set 
         *          and the precondition is not satisfied.
         *
         *  A call to set_headers takes a Range where each element models the
         *  Header concept. This Range will be linearized onto a buffer, which is
         *  then sent as soon as the first call to `write` or `flush` commences.
         */
        template <class Range>
        void set_headers(Range headers) {
            if (headers_already_set)
                boost::throw_exception(std::logic_error("Headers have already been set."));

            bool commit = false;
            BOOST_SCOPE_EXIT_TPL((&commit)(&headers_already_set))
            {
                if (!commit) {
                    headers_already_set = false;
                }
            } BOOST_SCOPE_EXIT_END

            typedef constants<Tag> consts;

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
            commit = headers_already_set = true;
        }

        void set_status(status_t new_status) {
            status = new_status;
        }

        template <class Range>
        void write(Range) {
            // linearize the range into a shared array
            // schedule a stranded asynchronous write
        }

        void flush() {
            // use this as a synchronization point to ensure
            // that data has been written; use a unique_future
        }

        void close() {
            flush();
            socket_.shutdown(asio::ip::tcp::socket::shutdown_both);
            socket_.close();
        }

        asio::ip::tcp::socket & socket()            { return socket_;               }
        utils::thread_pool & thread_pool()  { return thread_pool_;          }

    private:
        asio::ip::tcp::socket socket_;
        Handler & handler;
        utils::thread_pool & thread_pool_;
        bool headers_already_set;
        asio::streambuf headers_buffer;
        boost::uint16_t status;

        template <class, class> friend struct async_server_base;

        void start() {
            // FIXME do something!
        }

    };
    
} /* http */
    
} /* network */
    
} /* boost */

#endif /* BOOST_NETWORK_PROTOCOL_HTTP_SERVER_CONNECTION_HPP_20101027 */

