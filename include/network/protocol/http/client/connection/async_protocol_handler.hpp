#ifndef BOOST_NETWORK_PROTOCOL_HTTP_IMPL_HTTP_ASYNC_PROTOCOL_HANDLER_HPP_
#define BOOST_NETWORK_PROTOCOL_HTTP_IMPL_HTTP_ASYNC_PROTOCOL_HANDLER_HPP_

// Copyright 2010 (C) Dean Michael Berris
// Copyright 2011 Dean Michael Berris (dberris@google.com).
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <network/detail/debug.hpp>
#include <network/protocol/http/algorithms/linearize.hpp>

namespace boost { namespace network { namespace http { namespace impl {

  struct http_async_protocol_handler {
  protected:

#ifdef BOOST_NETWORK_DEBUG
    struct debug_escaper {
      std::string & string;
      explicit debug_escaper(std::string & string_)
        : string(string_) {}
      debug_escaper(debug_escaper const & other)
        : string(other.string) {}
      void operator()( std::string::value_type input) {
      if (!algorithm::is_print()(input)) {
        std::ostringstream escaped_stream;
        if (input == '\r') {
        string.append("\\r");
        } else if (input == '\n') {
        string.append("\\n");
        } else {
        escaped_stream << "\\x" << static_cast<int>(input);
        string.append(escaped_stream.str());
        }
      } else {
        string.push_back(input);
      }
      }
    };
#endif

    void init_response(response & response_) {
      boost::shared_future<std::string> source_future(
        source_promise.get_future());
      source(response_, source_future);
      boost::shared_future<std::string> destination_future(
        destination_promise.get_future());
      destination(response_, destination_future);
      boost::shared_future<std::multimap<std::string, std::string> >
        headers_future(headers_promise.get_future());
      headers(response_, headers_future);
      boost::shared_future<std::string> body_future(
        body_promise.get_future());
      body(response_, body_future);
      boost::shared_future<std::string> version_future(
        version_promise.get_future());
      version(response_, version_future);
      boost::shared_future<boost::uint16_t> status_future(
        status_promise.get_future());
      status(response_, status_future);
      boost::shared_future<std::string> status_message_future(
        status_message_promise.get_future());
      status_message(response_, status_message_future);
    }

    struct to_http_headers {
      template <class U>
      std::string const operator() (U const & pair) const {
        std::ostringstream header_line;
        header_line << pair.first
          << constants::colon()
          << constants::space()
          << pair.second
          << constants::crlf();
        return header_line.str();
      }
    };

    template <class Delegate, class Callback>
    logic::tribool parse_version(Delegate & delegate_,
                   Callback callback,
                   size_t bytes) {
      logic::tribool parsed_ok;
      part_begin = part.begin();
       buffer_type::const_iterator part_end = part.begin();
      std::advance(part_end, bytes);
       boost::iterator_range< buffer_type::const_iterator>
        result_range,
        input_range = boost::make_iterator_range(part_begin, part_end);
      fusion::tie(parsed_ok, result_range) = response_parser_.parse_until(
        response_parser::http_version_done,
        input_range);
      if (parsed_ok == true) {
        std::string version;
        std::swap(version, partial_parsed);
        version.append(boost::begin(result_range),
                 boost::end(result_range));
        algorithm::trim(version);
        version_promise.set_value(version);
        part_begin = boost::end(result_range);
      } else if (parsed_ok == false) {
#ifdef BOOST_NETWORK_DEBUG
        std::string escaped;
        debug_escaper escaper(escaped);
        std::for_each(part_begin, part_end, escaper);
        BOOST_NETWORK_MESSAGE("[parser:"
                    << response_parser_.state()
                    << "] buffer contents: \""
                    << escaped
                    << "\"");
#endif
        std::runtime_error error("Invalid Version Part.");
        version_promise.set_exception(boost::copy_exception(error));
        status_promise.set_exception(boost::copy_exception(error));
        status_message_promise.set_exception(
          boost::copy_exception(error));
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
        delegate_->read_some(
          boost::asio::mutable_buffers_1(part.c_array(), part.size()),
          callback
          );
      }
      return parsed_ok;
    }

    template <class Delegate, class Callback>
    logic::tribool parse_status(Delegate & delegate_,
                  Callback callback,
                  size_t bytes) {
      logic::tribool parsed_ok;
       buffer_type::const_iterator part_end = part.begin();
      std::advance(part_end, bytes);
       boost::iterator_range< buffer_type::const_iterator>
        result_range,
        input_range = boost::make_iterator_range(part_begin, part_end);
      fusion::tie(parsed_ok, result_range) = response_parser_.parse_until(
        response_parser::http_status_done,
        input_range);
      if (parsed_ok == true) {
        std::string status;
        std::swap(status, partial_parsed);
        status.append(boost::begin(result_range),
                boost::end(result_range));
        trim(status);
        boost::uint16_t status_int =
          lexical_cast<boost::uint16_t>(status);
        status_promise.set_value(status_int);
        part_begin = boost::end(result_range);
      } else if (parsed_ok == false) {
#ifdef BOOST_NETWORK_DEBUG
        std::string escaped;
        debug_escaper escaper(escaped);
        std::for_each(part_begin, part_end, escaper);
        BOOST_NETWORK_MESSAGE("[parser:"
                    << response_parser_.state()
                    << "] buffer contents: \""
                    << escaped
                    << "\"");
#endif
        std::runtime_error error("Invalid status part.");
        status_promise.set_exception(boost::copy_exception(error));
        status_message_promise.set_exception(
          boost::copy_exception(error));
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
        delegate_->read_some(
          boost::asio::mutable_buffers_1(part.c_array(), part.size()),
          callback
          );
      }
      return parsed_ok;
    }

    template <class Delegate, class Callback>
    logic::tribool parse_status_message(Delegate & delegate_,
                      Callback callback,
                      size_t bytes) {
      logic::tribool parsed_ok;
       buffer_type::const_iterator part_end = part.begin();
      std::advance(part_end, bytes);
       boost::iterator_range< buffer_type::const_iterator>
        result_range,
        input_range = boost::make_iterator_range(part_begin, part_end);
      fusion::tie(parsed_ok, result_range) = response_parser_.parse_until(
        response_parser::http_status_message_done,
        input_range);
      if (parsed_ok == true) {
        std::string status_message;
        std::swap(status_message, partial_parsed);
        status_message.append(boost::begin(result_range),
                    boost::end(result_range));
        algorithm::trim(status_message);
        status_message_promise.set_value(status_message);
        part_begin = boost::end(result_range);
      } else if (parsed_ok == false) {
#ifdef BOOST_NETWORK_DEBUG
        std::string escaped;
        debug_escaper escaper(escaped);
        std::for_each(part_begin, part_end, escaper);
        BOOST_NETWORK_MESSAGE("[parser:"
                    << response_parser_.state()
                    << "] buffer contents: \""
                    << escaped
                    << "\"");
#endif
        std::runtime_error error("Invalid status message part.");
        status_message_promise.set_exception(
          boost::copy_exception(error));
        headers_promise.set_exception(boost::copy_exception(error));
        source_promise.set_exception(boost::copy_exception(error));
        destination_promise.set_exception(boost::copy_exception(error));
        body_promise.set_exception(boost::copy_exception(error));
      } else {
        partial_parsed.append(
          boost::begin(result_range),
          boost::end(result_range));
        part_begin = part.begin();
        delegate_->read_some(
          boost::asio::mutable_buffers_1(part.c_array(), part.size()),
          callback
          );
      }
      return parsed_ok;
    }

    void parse_headers_real(std::string & headers_part) {
       boost::iterator_range< std::string::const_iterator>
        input_range = boost::make_iterator_range(headers_part)
        , result_range;
      logic::tribool parsed_ok;
      response_parser headers_parser(
        response_parser::http_header_line_done);
      std::multimap<std::string, std::string> headers;
      std::pair<std::string,std::string> header_pair;
      while (!boost::empty(input_range)) {
        fusion::tie(parsed_ok, result_range) =
          headers_parser.parse_until(
            response_parser::http_header_colon,
            input_range);
        if (headers_parser.state()
          != response_parser::http_header_colon)
          break;
        header_pair.first = std::string(boost::begin(result_range),
                        boost::end(result_range));
        input_range.advance_begin(boost::distance(result_range));
        fusion::tie(parsed_ok, result_range) =
          headers_parser.parse_until(
            response_parser::http_header_line_done,
            input_range);
        header_pair.second = std::string(boost::begin(result_range),
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

    template <class Delegate, class Callback>
    fusion::tuple<logic::tribool, size_t> parse_headers(Delegate & delegate_,
                              Callback callback,
                              size_t bytes) {
      logic::tribool parsed_ok;
       buffer_type::const_iterator part_end = part.begin();
      std::advance(part_end, bytes);
       boost::iterator_range< buffer_type::const_iterator>
        result_range,
        input_range = boost::make_iterator_range(part_begin, part_end);
      fusion::tie(parsed_ok, result_range) = response_parser_.parse_until(
        response_parser::http_headers_done,
        input_range);
      if (parsed_ok == true) {
        std::string headers_string;
        std::swap(headers_string, partial_parsed);
        headers_string.append(boost::begin(result_range),
                    boost::end(result_range));
        part_begin = boost::end(result_range);
        this->parse_headers_real(headers_string);
      } else if (parsed_ok == false) {
        // We want to output the contents of the buffer that caused
        // the error in debug builds.
#ifdef BOOST_NETWORK_DEBUG
        std::string escaped;
        debug_escaper escaper(escaped);
        std::for_each(part_begin, part_end, escaper);
        BOOST_NETWORK_MESSAGE("[parser:"
                    << response_parser_.state()
                    << "] buffer contents: \""
                    << escaped
                    << "\" consumed length: "
                    << boost::distance(result_range));
#endif
        std::runtime_error error("Invalid header part.");
        headers_promise.set_exception(boost::copy_exception(error));
        body_promise.set_exception(boost::copy_exception(error));
        source_promise.set_exception(boost::copy_exception(error));
        destination_promise.set_exception(boost::copy_exception(error));
      } else {
        partial_parsed.append(boost::begin(result_range),
                    boost::end(result_range));
        part_begin = part.begin();
        delegate_->read_some(
          boost::asio::mutable_buffers_1(part.c_array(), part.size()),
          callback
          );
      }
      return fusion::make_tuple(
        parsed_ok,
        std::distance(
          boost::end(result_range)
          , part_end
          )
        );
    }

    template <class Delegate, class Callback>
    void parse_body(Delegate & delegate_, Callback callback, size_t bytes) {
      // TODO: we should really not use a string for the partial body
      // buffer.
      partial_parsed.append(part_begin, bytes);
      part_begin = part.begin();
      delegate_->read_some(
        boost::asio::mutable_buffers_1(part.c_array(), part.size()),
        callback
        );
    }

    response_parser response_parser_;
    boost::promise<std::string> version_promise;
    boost::promise<boost::uint16_t> status_promise;
    boost::promise<std::string> status_message_promise;
    boost::promise<std::multimap<std::string, std::string> > headers_promise;
    boost::promise<std::string> source_promise;
    boost::promise<std::string> destination_promise;
    boost::promise<std::string> body_promise;
    typedef boost::array<char, BOOST_NETWORK_BUFFER_CHUNK> buffer_type;
    buffer_type part;
     buffer_type::const_iterator part_begin;
    std::string partial_parsed;
  };


} /* impl */

} /* http */

} /* network */

} /* boost */

#endif /* BOOST_NETWORK_PROTOCOL_HTTP_IMPL_HTTP_ASYNC_PROTOCOL_HANDLER_HPP_20101015 */
