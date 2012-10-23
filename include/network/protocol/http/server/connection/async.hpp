// Copyright 2010-2012 Dean Michael Berris <dberris@google.com>
// Copyright 2012 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef NETWORK_PROTOCOL_HTTP_SERVER_CONNECTION_ASYNC_HPP_20101027
#define NETWORK_PROTOCOL_HTTP_SERVER_CONNECTION_ASYNC_HPP_20101027

#include <boost/throw_exception.hpp>
#include <boost/scope_exit.hpp>
#include <network/protocol/http/request.hpp>
#include <network/protocol/http/algorithms/linearize.hpp>
#include <network/utils/thread_pool.hpp>
#include <boost/range/adaptor/sliced.hpp>
#include <boost/range/algorithm/transform.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <asio/ip/tcp.hpp>
#include <asio/streambuf.hpp>
#include <asio/strand.hpp>
#include <asio/buffer.hpp>
#include <asio/placeholders.hpp>
#include <asio/write.hpp>
#include <memory>
#include <network/protocol/http/server/request_parser.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/optional.hpp>
#include <boost/utility/typed_in_place_factory.hpp>
#include <thread>
#include <type_traits>
#include <list>
#include <vector>
#include <iterator>
#include <mutex>
#include <network/constants.hpp>

#ifndef NETWORK_HTTP_SERVER_CONNECTION_HEADER_BUFFER_MAX_SIZE
/** Here we define a page's worth of header connection buffer data.
 *  This can be tuned to reduce the memory cost of connections, but this 
 *  default size is set to be friendly to typical service applications.
 *  This is the maximum size though and Boost.Asio's internal representation
 *  of a streambuf would make appropriate decisions on how big a buffer
 *  is to begin with.
 *
 *  This kinda assumes that a page is by default 4096. Since we're using
 *  the default allocator with the static buffers, it's not guaranteed that
 *  the static buffers will be page-aligned when they are allocated.
 */
#define NETWORK_HTTP_SERVER_CONNECTION_HEADER_BUFFER_MAX_SIZE 4096
#endif /* NETWORK_HTTP_SERVER_CONNECTION_HEADER_BUFFER_MAX_SIZE */

#ifndef NETWORK_HTTP_SERVER_CONNECTION_BUFFER_SIZE
/** Here we're making the assumption again that the page size of the system
 *  is 4096 and that it's better to have page-aligned chunks when creating
 *  buffers for memory use efficiency.
 */
#define NETWORK_HTTP_SERVER_CONNECTION_BUFFER_SIZE 4096uL
#endif

namespace network { namespace http {

  extern void parse_version(std::string const & partial_parsed, boost::fusion::tuple<uint8_t,uint8_t> & version_pair);
  extern void parse_headers(std::string const & input, std::vector<std::pair<std::string,std::string> > & container);

  class async_server_connection : public std::enable_shared_from_this<async_server_connection> {
   public:
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

      typedef std::string string_type;
      typedef std::shared_ptr<async_server_connection> connection_ptr;

  private:
      static char const * status_message(status_t status) {
          static char const 
              ok_[]                       = "OK"
              , created_[]                = "Created"
              , accepted_[]               = "Accepted"
              , no_content_[]             = "No Content"
              , multiple_choices_[]       = "Multiple Choices"
              , moved_permanently_[]      = "Moved Permanently"
              , moved_temporarily_[]      = "Moved Temporarily"
              , not_modified_[]           = "Not Modified"
              , bad_request_[]            = "Bad Request"
              , unauthorized_[]           = "Unauthorized"
              , forbidden_[]              = "Fobidden"
              , not_found_[]              = "Not Found"
              , not_supported_[]          = "Not Supported"
              , not_acceptable_[]         = "Not Acceptable"
              , internal_server_error_[]  = "Internal Server Error"
              , not_implemented_[]        = "Not Implemented"
              , bad_gateway_[]            = "Bad Gateway"
              , service_unavailable_[]    = "Service Unavailable"
              , unknown_[]                = "Unknown"
              ;
          switch(status) {
              case ok:                    return ok_;
              case created:               return created_;
              case accepted:              return accepted_;
              case no_content:            return no_content_;
              case multiple_choices:      return multiple_choices_;
              case moved_permanently:     return moved_permanently_;
              case moved_temporarily:     return moved_temporarily_;
              case not_modified:          return not_modified_;
              case bad_request:           return bad_request_;
              case unauthorized:          return unauthorized_;
              case forbidden:             return forbidden_;
              case not_found:             return not_found_;
              case not_supported:         return not_supported_;
              case not_acceptable:        return not_acceptable_;
              case internal_server_error: return internal_server_error_;
              case not_implemented:       return not_implemented_;
              case bad_gateway:           return bad_gateway_;
              case service_unavailable:   return service_unavailable_;
              default:                    return unknown_;
          }
      }

  public:

      async_server_connection(
          asio::io_service & io_service
          , std::function<void(request const &, connection_ptr)> handler
          , utils::thread_pool & thread_pool
          )
      : socket_(io_service)
      , strand(io_service)
      , handler(handler)
      , thread_pool_(thread_pool)
      , headers_already_sent(false)
      , headers_in_progress(false)
      , headers_buffer(NETWORK_HTTP_SERVER_CONNECTION_HEADER_BUFFER_MAX_SIZE)
      {
          new_start = read_buffer_.begin();
      }

      ~async_server_connection() throw () {
          asio::error_code ignored;
          socket_.shutdown(asio::ip::tcp::socket::shutdown_receive, ignored);
      }

      /** Function: template <class Range> set_headers(Range headers)
       *  Precondition: headers have not been sent yet
       *  Postcondition: headers have been linearized to a buffer, 
       *                 and assumed to have been sent already when the function exits
       *  Throws: std::logic_error in case the headers have already been sent. 
       *
       *  A call to set_headers takes a Range where each element models the
       *  Header concept. This Range will be linearized onto a buffer, which is
       *  then sent as soon as the first call to `write` or `flush` commences.
       */
      template <class Range>
      void set_headers(Range headers) {
          lock_guard lock(headers_mutex);
          if (headers_in_progress || headers_already_sent) 
              boost::throw_exception(std::logic_error("Headers have already been sent."));

          if (error_encountered)
              boost::throw_exception(std::system_error(*error_encountered));

          {
              std::ostream stream(&headers_buffer);
              stream
                  << constants::http_slash() << 1<< constants::dot() << 1 << constants::space()
                  << status << constants::space() << status_message(status)
                  << constants::crlf();
              if (!boost::empty(headers)) {
                  typedef typename Range::const_iterator iterator;
                  boost::transform(headers, 
                      std::ostream_iterator<std::string>(stream),
                      linearize_header());
              } else {
                  stream << constants::crlf();
              }
              stream << constants::crlf();
          }

          write_headers_only(
              std::bind(
                  &async_server_connection::do_nothing
                  , async_server_connection::shared_from_this()
              ));
      }

      void set_status(status_t new_status) {
          lock_guard lock(headers_mutex);
          if (headers_already_sent) boost::throw_exception(std::logic_error("Headers have already been sent, cannot reset status."));
          if (error_encountered) boost::throw_exception(std::system_error(*error_encountered));

          status = new_status;
      }

      template <class Range>
      void write(Range const & range) {
          lock_guard lock(headers_mutex);
          if (error_encountered) boost::throw_exception(std::system_error(*error_encountered));
          using namespace std::placeholders;
          std::function<void(asio::error_code)> f = 
              std::bind(
                  &async_server_connection::default_error
                  , async_server_connection::shared_from_this()
                  , _1);

          write_impl(
              boost::make_iterator_range(range)
              , f
              );
      }

      template <class Range, class Callback>
      typename std::disable_if<std::is_base_of<asio::const_buffer, typename Range::value_type>>::type
      write(Range const & range, Callback const & callback) {
          lock_guard lock(headers_mutex);
          if (error_encountered) boost::throw_exception(std::system_error(*error_encountered));
          write_impl(boost::make_iterator_range(range), callback);
      }

      template <class ConstBufferSeq, class Callback>
      typename std::enable_if<std::is_base_of<asio::const_buffer, typename ConstBufferSeq::value_type>>::type
      write(ConstBufferSeq const & seq, Callback const & callback)
      {
          write_vec_impl(seq, callback, shared_array_list(), shared_buffers());
      }

  private:
      typedef std::array<char, NETWORK_HTTP_SERVER_CONNECTION_BUFFER_SIZE> buffer_type;

  public:
      typedef boost::iterator_range<buffer_type::const_iterator> input_range;
      typedef std::function<void(input_range, asio::error_code, std::size_t, connection_ptr)> read_callback_function;

      void read(read_callback_function callback) {
          if (error_encountered) boost::throw_exception(std::system_error(*error_encountered));
          if (new_start != read_buffer_.begin())
          {
              input_range input = boost::make_iterator_range(new_start, read_buffer_.end());
              thread_pool().post(
                  std::bind(
                      callback
                      , input
                      , asio::error_code()
                      , std::distance(new_start, data_end)
                      , async_server_connection::shared_from_this())
              );
              new_start = read_buffer_.begin();
              return;
          }

          socket().async_read_some(
              asio::buffer(read_buffer_)
              , strand.wrap(
                  std::bind(
                      &async_server_connection::wrap_read_handler
                      , async_server_connection::shared_from_this()
                      , callback
                      , asio::placeholders::error, asio::placeholders::bytes_transferred)));
      }

      asio::ip::tcp::socket & socket()    { return socket_;               }
      utils::thread_pool & thread_pool()  { return thread_pool_;          }
      bool has_error()                    { return (!!error_encountered); }
      boost::optional<std::system_error> error()
                                          { return error_encountered;     }

  private:

      void wrap_read_handler(read_callback_function callback, asio::error_code const & ec, std::size_t bytes_transferred) {
          if (ec) error_encountered = boost::in_place<std::system_error>(ec);
          buffer_type::const_iterator data_start = read_buffer_.begin()
                                     ,data_end   = read_buffer_.begin();
          std::advance(data_end, bytes_transferred);
          thread_pool().post(
              std::bind(
                  callback
                  , boost::make_iterator_range(data_start, data_end)
                  , ec
                  , bytes_transferred
                  , async_server_connection::shared_from_this()));
      }

      void default_error(asio::error_code const & ec) {
          error_encountered = boost::in_place<std::system_error>(ec);
      }

      typedef std::array<char, NETWORK_HTTP_SERVER_CONNECTION_BUFFER_SIZE> array;
      typedef std::list<std::shared_ptr<array> > array_list;
      typedef std::shared_ptr<array_list> shared_array_list;
      typedef std::shared_ptr<std::vector<asio::const_buffer> > shared_buffers;
      typedef std::lock_guard<std::recursive_mutex> lock_guard;
      typedef std::list<std::function<void()> > pending_actions_list;

      asio::ip::tcp::socket socket_;
      asio::io_service::strand strand;
      std::function<void(request const &, connection_ptr)> handler;
      utils::thread_pool & thread_pool_;
      volatile bool headers_already_sent, headers_in_progress;
      asio::streambuf headers_buffer;

      std::recursive_mutex headers_mutex;
      buffer_type read_buffer_;
      status_t status;
      request_parser parser;
      request request_;
      buffer_type::iterator new_start, data_end;
      std::string partial_parsed;
      boost::optional<std::system_error> error_encountered;
      pending_actions_list pending_actions;

      friend class async_server_impl;

      enum state_t {
          method, uri, version, headers
      };

      void start() {
          std::ostringstream ip_stream;
          ip_stream << socket_.remote_endpoint().address().to_string() << ':'
              << socket_.remote_endpoint().port();
          request_.set_source(ip_stream.str());
          read_more(method);
      }

      void read_more(state_t state) {
          socket_.async_read_some(
              asio::buffer(read_buffer_)
              , strand.wrap(
                  std::bind(
                      &async_server_connection::handle_read_data,
                      async_server_connection::shared_from_this(),
                      state,
                      asio::placeholders::error,
                      asio::placeholders::bytes_transferred
                      )
                  )
              );
      }

      void handle_read_data(state_t state, asio::error_code const & ec, std::size_t bytes_transferred) {
          if (!ec) {
              boost::logic::tribool parsed_ok;
              boost::iterator_range<buffer_type::iterator> result_range, input_range;
              data_end = read_buffer_.begin();
              std::advance(data_end, bytes_transferred);
              switch (state) {
                  case method:
                      input_range = boost::make_iterator_range(
                          new_start, data_end);
                      boost::fusion::tie(parsed_ok, result_range) = parser.parse_until(
                          request_parser::method_done, input_range);
                      if (!parsed_ok) { 
                          client_error();
                          break;
                      } else if (parsed_ok == true) {
                        std::string method;
                        swap(partial_parsed, method);
                        method.append(boost::begin(result_range),
                                      boost::end(result_range));
                        boost::trim(method);
                        request_.set_method(method);
                        new_start = boost::end(result_range);
                      } else {
                        partial_parsed.append(
                            boost::begin(result_range),
                            boost::end(result_range));
                        new_start = read_buffer_.begin();
                        read_more(method);
                        break;
                      }
                  case uri:
                      input_range = boost::make_iterator_range(
                          new_start, data_end);
                      boost::fusion::tie(parsed_ok, result_range) = parser.parse_until(
                          request_parser::uri_done,
                          input_range);
                      if (!parsed_ok) {
                          client_error();
                          break;
                      } else if (parsed_ok == true) {
                        std::string destination;
                        swap(partial_parsed, destination);
                        destination.append(boost::begin(result_range),
                                           boost::end(result_range));
                        boost::trim(destination);
                        request_.set_destination(destination);
                        new_start = boost::end(result_range);
                      } else {
                        partial_parsed.append(
                            boost::begin(result_range),
                            boost::end(result_range));
                        new_start = read_buffer_.begin();
                        read_more(uri);
                        break;
                      }
                  case version:
                      input_range = boost::make_iterator_range(
                          new_start, data_end);
                      boost::fusion::tie(parsed_ok, result_range) = parser.parse_until(
                          request_parser::version_done,
                          input_range);
                      if (!parsed_ok) {
                          client_error();
                          break;
                      } else if (parsed_ok == true) {
                          boost::fusion::tuple<uint8_t, uint8_t> version_pair;
                          partial_parsed.append(boost::begin(result_range), boost::end(result_range));
                          parse_version(partial_parsed, version_pair);
                          request_.set_version_major(boost::fusion::get<0>(version_pair));
                          request_.set_version_minor(boost::fusion::get<1>(version_pair));
                          new_start = boost::end(result_range);
                          partial_parsed.clear();
                      } else {
                          partial_parsed.append(
                              boost::begin(result_range),
                              boost::end(result_range));
                          new_start = read_buffer_.begin();
                          read_more(version);
                          break;
                      }
                  case headers:
                      input_range = boost::make_iterator_range(
                          new_start, data_end);
                      boost::fusion::tie(parsed_ok, result_range) = parser.parse_until(
                          request_parser::headers_done,
                          input_range);
                      if (!parsed_ok) {
                          client_error();
                          break;
                      } else if (parsed_ok == true) {
                          partial_parsed.append(
                              boost::begin(result_range),
                              boost::end(result_range));
                          std::vector<std::pair<std::string, std::string> > headers;
                          parse_headers(partial_parsed, headers);
                          for (std::vector<std::pair<std::string, std::string> >::const_iterator it = headers.begin();
                               it != headers.end();
                               ++it) {
                            request_.append_header(it->first, it->second);
                          }
                          new_start = boost::end(result_range);
                          thread_pool().post(
                              std::bind(
                                  handler,
                                  boost::cref(request_),
                                  async_server_connection::shared_from_this()));
                          return;
                      } else {
                          partial_parsed.append(
                              boost::begin(result_range),
                              boost::end(result_range));
                          new_start = read_buffer_.begin();
                          read_more(headers);
                          break;
                      }
                  default:
                      BOOST_ASSERT(false && "This is a bug, report to the cpp-netlib devel mailing list!");
                      std::abort();
              }
          } else {
              error_encountered = boost::in_place<std::system_error>(ec);
          }
      }

      void client_error() {
          static char const * bad_request = 
              "HTTP/1.0 400 Bad Request\r\nConnection: close\r\nContent-Type: text/plain\r\nContent-Length: 12\r\n\r\nBad Request.";

          asio::async_write(
              socket()
              , asio::buffer(bad_request, strlen(bad_request))
              , strand.wrap(
                  std::bind(
                      &async_server_connection::client_error_sent
                      , async_server_connection::shared_from_this()
                      , asio::placeholders::error
                      , asio::placeholders::bytes_transferred)));
      }

      void client_error_sent(asio::error_code const & ec, std::size_t bytes_transferred) {
          if (!ec) {
              asio::error_code ignored;
              socket().shutdown(asio::ip::tcp::socket::shutdown_both, ignored);
              socket().close(ignored);
          } else {
              error_encountered = boost::in_place<std::system_error>(ec);
          }
      }

      void do_nothing() {}

      void write_headers_only(std::function<void()> callback) {
          if (headers_in_progress) return;
          headers_in_progress = true;
          asio::async_write(
              socket()
              , headers_buffer
              , strand.wrap(
                  std::bind(
                      &async_server_connection::handle_write_headers
                      , async_server_connection::shared_from_this()
                      , callback
                      , asio::placeholders::error
                      , asio::placeholders::bytes_transferred)));
      }

      void handle_write_headers(std::function<void()> callback, asio::error_code const & ec, std::size_t bytes_transferred) {
          lock_guard lock(headers_mutex);
          if (!ec) {
              headers_buffer.consume(headers_buffer.size());
              headers_already_sent = true;
              thread_pool().post(callback);
              pending_actions_list::iterator start = pending_actions.begin()
                  , end = pending_actions.end();
              while (start != end) {
                  thread_pool().post(*start++);
              }
              pending_actions_list().swap(pending_actions);
          } else {
              error_encountered = boost::in_place<std::system_error>(ec);
          }
      }

      void handle_write(
          std::function<void(asio::error_code const &)> callback
          , shared_array_list temporaries
          , shared_buffers buffers
          , asio::error_code const & ec
          , std::size_t bytes_transferred
      ) {
          // we want to forget the temporaries and buffers
          thread_pool().post(std::bind(callback, ec));
      }

      template <class Range>
      void write_impl(Range range, std::function<void(asio::error_code)> callback) {
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

          static std::size_t const connection_buffer_size =
              NETWORK_HTTP_SERVER_CONNECTION_BUFFER_SIZE;
          shared_array_list temporaries = 
              std::make_shared<array_list>();
          shared_buffers buffers = 
              std::make_shared<std::vector<asio::const_buffer> >(0);

          std::size_t range_size = boost::distance(range);
          buffers->reserve(
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
              std::shared_ptr<array> new_array = std::make_shared<array>();
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
              write_vec_impl(*buffers, callback, temporaries, buffers);
          }
      }

      template <class ConstBufferSeq, class Callback>
      void write_vec_impl(ConstBufferSeq const & seq
                         ,Callback const & callback
                         ,shared_array_list temporaries
                         ,shared_buffers buffers)
      {
          lock_guard lock(headers_mutex);
          if (error_encountered)
              boost::throw_exception(std::system_error(*error_encountered));

          std::function<void(asio::error_code)> callback_function =
                  callback;

          std::function<void()> continuation = std::bind(
              &async_server_connection::template write_vec_impl<ConstBufferSeq, std::function<void(asio::error_code)> >
              ,async_server_connection::shared_from_this()
              ,seq, callback_function, temporaries, buffers
          );

          if (!headers_already_sent && !headers_in_progress) {
              write_headers_only(continuation);
              return;
          } else if (headers_in_progress && !headers_already_sent) {
              pending_actions.push_back(continuation);
              return;
          }

          asio::async_write(
               socket_
              ,seq
              ,std::bind(
                  &async_server_connection::handle_write
                  ,async_server_connection::shared_from_this()
                  ,callback_function
                  ,temporaries
                  ,buffers
                  ,asio::placeholders::error
                  ,asio::placeholders::bytes_transferred)
          );
      }
  };
  
}  // namespace http
}  // namespace network
  
#endif /* NETWORK_PROTOCOL_HTTP_SERVER_CONNECTION_HPP_20101027 */
