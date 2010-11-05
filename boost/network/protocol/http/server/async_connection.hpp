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
#include <boost/range/adaptor/sliced.hpp>
#include <boost/range/algorithm/transform.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/asio/strand.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/make_shared.hpp>
#include <list>
#include <vector>
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
        static std::size_t const connection_buffer_size = BOOST_NETWORK_HTTP_SERVER_CONNECTION_BUFFER_SIZE;

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

        typedef typename string<Tag>::type string_type;

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
        void set_headers(Range headers, bool immediate = true) {
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
            if (immediate) write_headers_only();

            commit = true;
        }

        void set_status(status_t new_status) {
            status = new_status;
        }

        template <class Range>
        void write(Range const & range) {
            write_impl(
                boost::make_iterator_range(range)
                , boost::bind(
                    &async_connection<Tag,Handler>::default_error
                    , async_connection<Tag,Handler>::shared_from_this()
                    , _1
                    )
                );
        }

        template <class Range, class Callback>
        void write(Range const & range, Callback const & callback) {
            write_impl(
                boost::make_iterator_range(range)
                , callback
                );
        }

        asio::ip::tcp::socket & socket()    { return socket_;               }
        utils::thread_pool & thread_pool()  { return thread_pool_;          }

    private:

        void default_error(boost::system::error_code const & ec) {
            // TODO implement a sane default here, for now ignore the error
        }

        asio::ip::tcp::socket socket_;
        asio::io_service::strand strand;
        Handler & handler;
        utils::thread_pool & thread_pool_;
        bool headers_already_sent;
        asio::streambuf headers_buffer;

        typedef boost::array<char, connection_buffer_size> 
            buffer_type;
        typedef boost::array<char, connection_buffer_size> 
            array;
        typedef std::list<shared_ptr<array> > array_list;
        typedef boost::shared_ptr<array_list> shared_array_list;
        typedef boost::shared_ptr<std::vector<asio::const_buffer> > shared_buffers;
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

        template <class Range, class Callback>
        void write_headers(Range range, Callback callback) {
            // TODO send out the headers, then once that's done 
            // call the write again on the range and callback
        }

        void write_headers_only() {
        }

        void handle_write_headers(boost::system::error_code const & ec) {
            if (ec) {
                // TODO signal somehow that there was an error so that subsequent
                // calls to write would throw an exception
                return;
            }
            headers_already_sent = true;
        }

        void handle_write(
            boost::function<void(boost::system::error_code const &)> callback
            , shared_array_list temporaries
            , shared_buffers buffers
            , boost::system::error_code const & ec
        ) {
            // we want to forget the temporaries and buffers
            thread_pool().post(boost::bind(callback, ec));
        }

        template <class Range, class Callback>
        void write_impl(Range range, Callback callback) {
            if (!headers_already_sent) {
                write_headers(range, callback);
                return;
            }

            // linearize the whole range into a vector
            // of fixed-sized buffers, then schedule an asynchronous
            // write of these buffers -- make sure they are live
            // by making these linearized buffers shared and made
            // part of the completion handler.
            //
            // once the range has been linearized and sent, schedule
            // a wrapper to be called in the io_service's thread, that
            // will re-schedule the given callback into the thread pool
            // referred to here so that the io_service's thread can concentrate
            // on doing I/O.
            //

            shared_array_list temporaries = 
                boost::make_shared<array_list>();
            shared_buffers buffers = 
                boost::make_shared<std::vector<asio::const_buffer> >(0);

            std::size_t range_size = boost::distance(range);
            buffers->resize(
                (range_size / connection_buffer_size)
                + ((range_size % connection_buffer_size)?1:0)
                );
            std::size_t slice_size = 
                std::min(range_size,connection_buffer_size);
            typename boost::range_iterator<Range>::type
                start = boost::begin(range)
                , end  = boost::end(range);
            while (slice_size != 0) {
                using boost::adaptors::sliced;
                shared_ptr<array> new_array = make_shared<array>();
                boost::copy(
                    range | sliced(0,slice_size)
                    , new_array->begin()
                    );
                temporaries->push_back(new_array);
                buffers->push_back(asio::buffer(new_array->data(), slice_size));
                std::advance(start, slice_size);
                range = boost::make_iterator_range(start, end);
                range_size = boost::distance(range);
                slice_size = std::min(range_size, connection_buffer_size);
            }

            if (!buffers->empty()) {
                boost::function<void(boost::system::error_code const &)> f = callback;
                asio::async_write(
                    socket_
                    , *buffers
                    , strand.wrap(
                        boost::bind(
                            &async_connection<Tag,Handler>::handle_write
                            , async_connection<Tag,Handler>::shared_from_this()
                            , f
                            , temporaries
                            , buffers // keep these alive until the handler is called!
                            , boost::asio::placeholders::error
                            )
                        )
                    );
            }
        }

    };
    
} /* http */
    
} /* network */
    
} /* boost */

#endif /* BOOST_NETWORK_PROTOCOL_HTTP_SERVER_CONNECTION_HPP_20101027 */

