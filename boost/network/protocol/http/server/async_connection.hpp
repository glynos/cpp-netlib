#ifndef BOOST_NETWORK_PROTOCOL_HTTP_SERVER_CONNECTION_HPP_20101027
#define BOOST_NETWORK_PROTOCOL_HTTP_SERVER_CONNECTION_HPP_20101027

// Copyright 2010 Dean Michael Berris.
// Copyright 2014 Jelle Van den Driessche.
// Copyright 2015 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <iterator>
#include <list>
#include <vector>
#include <memory>
#include <mutex>
#include <array>
#include <functional>
#include <tuple>
#include <asio/buffer.hpp>
#include <asio/ip/tcp.hpp>
#include <asio/strand.hpp>
#include <asio/streambuf.hpp>
#include <boost/network/protocol/http/algorithms/linearize.hpp>
#include <boost/network/protocol/http/server/request_parser.hpp>
#include <boost/network/protocol/stream_handler.hpp>
#include <boost/network/utils/thread_pool.hpp>
#include <boost/optional.hpp>
#include <boost/range/adaptor/sliced.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/range/algorithm/transform.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/scope_exit.hpp>
#include <boost/throw_exception.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/utility/typed_in_place_factory.hpp>

#ifndef BOOST_NETWORK_HTTP_SERVER_CONNECTION_HEADER_BUFFER_MAX_SIZE
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
#define BOOST_NETWORK_HTTP_SERVER_CONNECTION_HEADER_BUFFER_MAX_SIZE 4096
#endif /* BOOST_NETWORK_HTTP_SERVER_CONNECTION_HEADER_BUFFER_MAX_SIZE */

#ifndef BOOST_NETWORK_HTTP_SERVER_CONNECTION_BUFFER_SIZE
/**
 * We define the buffer size for each connection that we will use on the server
 * side.
 */
#define BOOST_NETWORK_HTTP_SERVER_CONNECTION_BUFFER_SIZE 1024uL
#endif

namespace boost {
namespace network {
namespace http {

#ifndef BOOST_NETWORK_NO_LIB
extern void parse_version(std::string const& partial_parsed,
                          std::tuple<std::uint8_t, std::uint8_t>& version_pair);
extern void parse_headers(std::string const& input,
                          std::vector<request_header_narrow>& container);
#endif

template <class Tag, class Handler>
struct async_connection
    : std::enable_shared_from_this<async_connection<Tag, Handler> > {

  enum status_t {
    ok = 200,
    created = 201,
    accepted = 202,
    no_content = 204,
    partial_content = 206,
    multiple_choices = 300,
    moved_permanently = 301,
    moved_temporarily = 302,
    not_modified = 304,
    bad_request = 400,
    unauthorized = 401,
    forbidden = 403,
    not_found = 404,
    not_supported = 405,
    not_acceptable = 406,
    request_timeout = 408,
    precondition_failed = 412,
    unsatisfiable_range = 416,
    internal_server_error = 500,
    not_implemented = 501,
    bad_gateway = 502,
    service_unavailable = 503,
    space_unavailable = 507
  };

  typedef typename string<Tag>::type string_type;
  typedef basic_request<Tag> request;
  typedef std::shared_ptr<async_connection> connection_ptr;

 private:
  static char const* status_message(status_t status) {
    static char const ok_[] = "OK", created_[] = "Created",
                      accepted_[] = "Accepted", no_content_[] = "No Content",
                      multiple_choices_[] = "Multiple Choices",
                      moved_permanently_[] = "Moved Permanently",
                      moved_temporarily_[] = "Moved Temporarily",
                      not_modified_[] = "Not Modified",
                      bad_request_[] = "Bad Request",
                      unauthorized_[] = "Unauthorized",
                      forbidden_[] = "Fobidden", not_found_[] = "Not Found",
                      not_supported_[] = "Not Supported",
                      not_acceptable_[] = "Not Acceptable",
                      internal_server_error_[] = "Internal Server Error",
                      not_implemented_[] = "Not Implemented",
                      bad_gateway_[] = "Bad Gateway",
                      service_unavailable_[] = "Service Unavailable",
                      unknown_[] = "Unknown",
                      partial_content_[] = "Partial Content",
                      request_timeout_[] = "Request Timeout",
                      precondition_failed_[] = "Precondition Failed",
                      unsatisfiable_range_[] =
                          "Requested Range Not Satisfiable",
                      space_unavailable_[] =
                          "Insufficient Space to Store Resource";
    switch (status) {
      case ok:
        return ok_;
      case created:
        return created_;
      case accepted:
        return accepted_;
      case no_content:
        return no_content_;
      case multiple_choices:
        return multiple_choices_;
      case moved_permanently:
        return moved_permanently_;
      case moved_temporarily:
        return moved_temporarily_;
      case not_modified:
        return not_modified_;
      case bad_request:
        return bad_request_;
      case unauthorized:
        return unauthorized_;
      case forbidden:
        return forbidden_;
      case not_found:
        return not_found_;
      case not_supported:
        return not_supported_;
      case not_acceptable:
        return not_acceptable_;
      case internal_server_error:
        return internal_server_error_;
      case not_implemented:
        return not_implemented_;
      case bad_gateway:
        return bad_gateway_;
      case service_unavailable:
        return service_unavailable_;
      case partial_content:
        return partial_content_;
      case request_timeout:
        return request_timeout_;
      case precondition_failed:
        return precondition_failed_;
      case unsatisfiable_range:
        return unsatisfiable_range_;
      case space_unavailable:
        return space_unavailable_;
      default:
        return unknown_;
    }
  }

 public:
  async_connection(asio::io_service& io_service, Handler& handler,
                   utils::thread_pool& thread_pool,
                   std::shared_ptr<ssl_context> ctx =
                       std::shared_ptr<ssl_context>())
      : strand(io_service),
        handler(handler),
        thread_pool_(thread_pool),
        headers_buffer(
            BOOST_NETWORK_HTTP_SERVER_CONNECTION_HEADER_BUFFER_MAX_SIZE),
#ifdef BOOST_NETWORK_ENABLE_HTTPS
        socket_(io_service, ctx),
#else
        socket_(io_service),
#endif
        handshake_done(false),
        headers_already_sent(false),
        headers_in_progress(false) {
    (void)ctx;
    new_start = read_buffer_.begin();
  }

  ~async_connection() throw() {
    std::error_code ignored;
    socket_.shutdown(asio::ip::tcp::socket::shutdown_receive, ignored);
  }

  /** Function: template <class Range> set_headers(Range headers)
   *  Precondition: headers have not been sent yet
   *  Postcondition: headers have been linearized to a buffer,
   *                 and assumed to have been sent already when the
   *function exits
   *  Throws: std::logic_error in case the headers have already been sent.
   *
   *  A call to set_headers takes a Range where each element models the
   *  Header concept. This Range will be linearized onto a buffer, which
   *is
   *  then sent as soon as the first call to `write` or `flush` commences.
   */
  template <class Range>
  void set_headers(Range headers) {
    lock_guard lock(headers_mutex);
    if (headers_in_progress || headers_already_sent)
      boost::throw_exception(
          std::logic_error("Headers have already been sent."));

    if (error_encountered)
      boost::throw_exception(std::system_error(*error_encountered));

    typedef constants<Tag> consts;
    {
      std::ostream stream(&headers_buffer);
      stream << consts::http_slash() << 1 << consts::dot() << 1
             << consts::space() << status << consts::space()
             << status_message(status) << consts::crlf();
      if (!boost::empty(headers)) {
        typedef typename string<Tag>::type string_type;
        boost::transform(headers, std::ostream_iterator<string_type>(stream),
                         linearize_header<Tag>());
      } else {
        stream << consts::crlf();
      }
      stream << consts::crlf();
    }

    auto self = this->shared_from_this();
    write_headers_only([self] {});
  }

  void set_status(status_t new_status) {
    lock_guard lock(headers_mutex);
    if (headers_already_sent)
      boost::throw_exception(std::logic_error(
          "Headers have already been sent, cannot reset status."));
    if (error_encountered)
      boost::throw_exception(std::system_error(*error_encountered));

    status = new_status;
  }

  template <class Range>
  void write(Range const& range) {
    lock_guard lock(headers_mutex);
    if (error_encountered)
      boost::throw_exception(std::system_error(*error_encountered));
    auto self = this->shared_from_this();
    auto f = [this, self](std::error_code ec) {
      this->default_error(ec);
    };
    write_impl(boost::make_iterator_range(range), f);
  }

  template <class Range, class Callback>
  typename disable_if<
      is_base_of<asio::const_buffer, typename Range::value_type>, void>::type
  write(Range const& range, Callback const& callback) {
    lock_guard lock(headers_mutex);
    if (error_encountered)
      boost::throw_exception(std::system_error(*error_encountered));
    write_impl(boost::make_iterator_range(range), callback);
  }

  template <class ConstBufferSeq, class Callback>
  typename enable_if<
      is_base_of<asio::const_buffer, typename ConstBufferSeq::value_type>,
      void>::type
  write(ConstBufferSeq const& seq, Callback const& callback) {
    write_vec_impl(seq, callback, shared_array_list(), shared_buffers());
  }

 private:
  typedef std::array<char, BOOST_NETWORK_HTTP_SERVER_CONNECTION_BUFFER_SIZE>
      buffer_type;

 public:
  typedef iterator_range<buffer_type::const_iterator> input_range;
  typedef std::function<
      void(input_range, std::error_code, std::size_t, connection_ptr)>
      read_callback_function;

  void read(read_callback_function callback) {
    if (error_encountered)
      boost::throw_exception(std::system_error(*error_encountered));
    if (new_start != read_buffer_.begin()) {
      input_range input =
          boost::make_iterator_range(new_start, read_buffer_.end());
      buffer_type::iterator start_tmp = new_start;
      new_start = read_buffer_.begin();
      auto self = this->shared_from_this();
      thread_pool().post([this, self, callback, input, start_tmp] {
        callback(input, {}, std::distance(start_tmp, data_end), self);
      });
      return;
    }

    auto self = this->shared_from_this();
    socket().async_read_some(
        asio::buffer(read_buffer_),
        strand.wrap([this, self, callback](std::error_code ec,
                                           size_t bytes_transferred) {
          callback(ec, bytes_transferred);
        }));
  }

  boost::network::stream_handler& socket() { return socket_; }
  utils::thread_pool& thread_pool() { return thread_pool_; }
  bool has_error() { return (!!error_encountered); }
  optional<std::system_error> error() { return error_encountered; }

 private:
  void wrap_read_handler(read_callback_function callback,
                         std::error_code const& ec,
                         std::size_t bytes_transferred) {
    if (ec) error_encountered = in_place<std::system_error>(ec);
    buffer_type::const_iterator data_start = read_buffer_.begin(),
                                data_end = read_buffer_.begin();
    std::advance(data_end, bytes_transferred);
    auto range = boost::make_iterator_range(data_start, data_end);
    auto self = this->shared_from_this();
    thread_pool().post([callback, range, ec, bytes_transferred, self] {
      callback(range, ec, bytes_transferred, self);
    });
  }

  void default_error(std::error_code const& ec) {
    if (ec) error_encountered = in_place<std::system_error>(ec);
  }

  typedef std::array<char, BOOST_NETWORK_HTTP_SERVER_CONNECTION_BUFFER_SIZE>
      array;
  typedef std::list<std::shared_ptr<array> > array_list;
  typedef std::shared_ptr<array_list> shared_array_list;
  typedef std::shared_ptr<std::vector<asio::const_buffer> > shared_buffers;
  typedef request_parser<Tag> request_parser_type;
  typedef std::lock_guard<std::recursive_mutex> lock_guard;
  typedef std::list<std::function<void()> > pending_actions_list;

  asio::io_service::strand strand;
  Handler& handler;
  utils::thread_pool& thread_pool_;
  asio::streambuf headers_buffer;
  boost::network::stream_handler socket_;
  bool handshake_done;
  volatile bool headers_already_sent, headers_in_progress;

  std::recursive_mutex headers_mutex;
  buffer_type read_buffer_;
  status_t status;
  request_parser_type parser;
  request request_;
  buffer_type::iterator new_start, data_end;
  string_type partial_parsed;
  optional<std::system_error> error_encountered;
  pending_actions_list pending_actions;

  template <class, class>
  friend struct async_server_base;

  enum state_t {
    method,
    uri,
    version,
    headers
  };

  void start() {
    typename ostringstream<Tag>::type ip_stream;
    ip_stream << socket_.remote_endpoint().address().to_v4().to_string() << ':'
              << socket_.remote_endpoint().port();
    request_.source = ip_stream.str();
    read_more(method);
  }

  void read_more(state_t state) {
    auto self = this->shared_from_this();
#ifdef BOOST_NETWORK_ENABLE_HTTPS
    if (socket_.is_ssl_enabled() && !handshake_done) {
      socket_.async_handshake(
          asio::ssl::stream_base::server,
          [this, self, state](std::error_code ec) {
            handle_handshake(ec, state);
          });
    } else {
#endif
      socket_.async_read_some(
          asio::buffer(read_buffer_),
          strand.wrap([this, self, state](std::error_code ec,
                                          size_t bytes_transferred) {
            handle_read_data(state, ec, bytes_transferred);
          }));
#ifdef BOOST_NETWORK_ENABLE_HTTPS
    }
#endif
  }

  void handle_read_data(state_t state, std::error_code const& ec,
                        std::size_t bytes_transferred) {
    if (!ec) {
      logic::tribool parsed_ok;
      iterator_range<buffer_type::iterator> result_range, input_range;
      data_end = read_buffer_.begin();
      std::advance(data_end, bytes_transferred);
      switch (state) {
        case method:
          input_range = boost::make_iterator_range(new_start, data_end);
          std::tie(parsed_ok, result_range) =
              parser.parse_until(request_parser_type::method_done, input_range);
          if (!parsed_ok) {
            client_error();
            break;
          } else if (parsed_ok == true) {
            swap(partial_parsed, request_.method);
            request_.method.append(std::begin(result_range),
                                   std::end(result_range));
            trim(request_.method);
            new_start = std::end(result_range);
          } else {
            partial_parsed.append(std::begin(result_range),
                                  std::end(result_range));
            new_start = read_buffer_.begin();
            read_more(method);
            break;
          }
        case uri:
          input_range = boost::make_iterator_range(new_start, data_end);
          std::tie(parsed_ok, result_range) =
              parser.parse_until(request_parser_type::uri_done, input_range);
          if (!parsed_ok) {
            client_error();
            break;
          } else if (parsed_ok == true) {
            swap(partial_parsed, request_.destination);
            request_.destination.append(std::begin(result_range),
                                        std::end(result_range));
            trim(request_.destination);
            new_start = std::end(result_range);
          } else {
            partial_parsed.append(std::begin(result_range),
                                  std::end(result_range));
            new_start = read_buffer_.begin();
            read_more(uri);
            break;
          }
        case version:
          input_range = boost::make_iterator_range(new_start, data_end);
          std::tie(parsed_ok, result_range) = parser.parse_until(
              request_parser_type::version_done, input_range);
          if (!parsed_ok) {
            client_error();
            break;
          } else if (parsed_ok == true) {
            std::tuple<std::uint8_t, std::uint8_t> version_pair;
            partial_parsed.append(std::begin(result_range),
                                  std::end(result_range));
            parse_version(partial_parsed, version_pair);
            request_.http_version_major = std::get<0>(version_pair);
            request_.http_version_minor = std::get<1>(version_pair);
            new_start = std::end(result_range);
            partial_parsed.clear();
          } else {
            partial_parsed.append(std::begin(result_range),
                                  std::end(result_range));
            new_start = read_buffer_.begin();
            read_more(version);
            break;
          }
        case headers:
          input_range = boost::make_iterator_range(new_start, data_end);
          std::tie(parsed_ok, result_range) = parser.parse_until(
              request_parser_type::headers_done, input_range);
          if (!parsed_ok) {
            client_error();
            break;
          } else if (parsed_ok == true) {
            partial_parsed.append(std::begin(result_range),
                                  std::end(result_range));
            try {
              parse_headers(partial_parsed, request_.headers);
            } catch (...) {
              client_error();
              break;
            }
            new_start = std::end(result_range);
            auto self = this->shared_from_this();
            thread_pool().post([this, self] { handler(request_, self); });
            return;
          } else {
            partial_parsed.append(std::begin(result_range),
                                  std::end(result_range));
            new_start = read_buffer_.begin();
            read_more(headers);
            break;
          }
        default:
          BOOST_ASSERT(false &&
                       "This is a bug, report to the cpp-netlib devel "
                       "mailing list!");
          std::abort();
      }
    } else {
      error_encountered = in_place<std::system_error>(ec);
    }
  }

  void client_error() {
    static char const bad_request[] =
        "HTTP/1.0 400 Bad Request\r\nConnection: close\r\nContent-Type: "
        "text/plain\r\nContent-Length: 12\r\n\r\nBad Request.";

    auto self = this->shared_from_this();
    asio::async_write(socket(), asio::buffer(bad_request, strlen(bad_request)),
                      strand.wrap([this, self](std::error_code ec,
                                               size_t bytes_transferred) {
                        client_error_sent(ec, bytes_transferred);
                      }));
  }

  void client_error_sent(std::error_code const& ec, std::size_t) {
    if (!ec) {
      std::error_code ignored;
      socket().shutdown(asio::ip::tcp::socket::shutdown_both, ignored);
      socket().close(ignored);
    } else {
      error_encountered = in_place<std::system_error>(ec);
    }
  }

  void write_headers_only(std::function<void()> callback) {
    if (headers_in_progress) return;
    headers_in_progress = true;
    auto self = this->shared_from_this();
    asio::async_write(
        socket(), headers_buffer,
        strand.wrap([this, self, callback] (std::error_code ec, size_t bytes_transferred) {
	  handle_write_headers(callback, ec, bytes_transferred);
	  }));
  }

  void handle_write_headers(std::function<void()> callback,
                            std::error_code const& ec, std::size_t) {
    lock_guard lock(headers_mutex);
    if (!ec) {
      headers_buffer.consume(headers_buffer.size());
      headers_already_sent = true;
      thread_pool().post(callback);
      auto start = pending_actions.begin(), end = pending_actions.end();
      while (start != end) {
        thread_pool().post(*start++);
      }
      pending_actions_list().swap(pending_actions);
    } else {
      error_encountered = in_place<std::system_error>(ec);
    }
  }

  void handle_write(
      std::function<void(std::error_code const&)> callback,
      shared_array_list, shared_buffers, std::error_code const& ec,
      std::size_t) {
    // we want to forget the temporaries and buffers
    thread_pool().post([callback, ec] { callback(ec); });
  }

  template <class Range>
  void write_impl(Range range,
                  std::function<void(std::error_code)> callback) {
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
        BOOST_NETWORK_HTTP_SERVER_CONNECTION_BUFFER_SIZE;
    shared_array_list temporaries = std::make_shared<array_list>();
    shared_buffers buffers =
        std::make_shared<std::vector<asio::const_buffer> >(0);

    std::size_t range_size = boost::distance(range);
    buffers->reserve((range_size / connection_buffer_size) +
                     ((range_size % connection_buffer_size) ? 1 : 0));
    std::size_t slice_size = std::min(range_size, connection_buffer_size);
    auto start = std::begin(range), end = std::end(range);
    while (slice_size != 0) {
      using boost::adaptors::sliced;
      std::shared_ptr<array> new_array = std::make_shared<array>();
      boost::copy(range | sliced(0, slice_size), new_array->begin());
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
  void write_vec_impl(ConstBufferSeq const& seq, Callback const& callback,
                      shared_array_list temporaries, shared_buffers buffers) {
    lock_guard lock(headers_mutex);
    if (error_encountered)
      boost::throw_exception(std::system_error(*error_encountered));
    auto self = this->shared_from_this();
    auto continuation = [this, self, seq, callback, temporaries, buffers] {
      write_vec_impl(seq, callback, temporaries, buffers);
    };
    if (!headers_already_sent && !headers_in_progress) {
      write_headers_only(continuation);
      return;
    }
    if (headers_in_progress && !headers_already_sent) {
      pending_actions.push_back(continuation);
      return;
    }
    asio::async_write(socket_, seq, [this, self, callback, temporaries,
                                     buffers](std::error_code ec,
                                              size_t bytes_transferred) {
      handle_write(callback, temporaries, buffers, ec, bytes_transferred);
    });
  }

  void handle_handshake(const std::error_code& ec, state_t state) {
    if (!ec) {
      handshake_done = true;
      read_more(state);
    } else {
      error_encountered = in_place<std::system_error>(ec);
    }
  }
};

}  // namespace http
}  // namespace network
}  // namespace boost

#endif  // BOOST_NETWORK_PROTOCOL_HTTP_SERVER_CONNECTION_HPP_20101027
