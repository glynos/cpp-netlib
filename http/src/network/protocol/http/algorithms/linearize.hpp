// Copyright 2010 Dean Michael Berris.
// Copyright 2012 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef NETWORK_PROTOCOL_HTTP_ALGORITHMS_LINEARIZE_HPP_20101028
#define NETWORK_PROTOCOL_HTTP_ALGORITHMS_LINEARIZE_HPP_20101028

#include <network/protocol/http/message/header/name.hpp>
#include <network/protocol/http/message/header/value.hpp>
#include <network/protocol/http/message/header_concept.hpp>
#include <network/protocol/http/request/request_concept.hpp>
#include <network/constants.hpp>
#include <boost/concept/requires.hpp>
#include <boost/optional.hpp>
#include <boost/range/algorithm/copy.hpp>

namespace network {
namespace http {

struct linearize_header {
  typedef std::string string_type;

  template <class Arguments>
  struct result;

  template <class This, class Arg>
  struct result<This(Arg)> {
      typedef string_type type;
  };

  template <class ValueType>
  BOOST_CONCEPT_REQUIRES(
      ((Header<ValueType>)),
      (string_type)
  ) operator()(ValueType const & header) {
      typedef std::ostringstream output_stream;
      typedef constants consts;
      output_stream header_line;
      header_line << name(header) 
          << consts::colon() << consts::space() 
          << value(header) << consts::crlf();
      return header_line.str();
  }
};

template <class Request, class OutputIterator>
BOOST_CONCEPT_REQUIRES(
    ((ClientRequest<Request>)),
    (OutputIterator)
) linearize(
    Request const & request, 
    std::string const & method,
    unsigned version_major, 
    unsigned version_minor, 
    OutputIterator oi
    ) 
{
    typedef constants consts;
    typedef std::string string_type;
    static string_type 
        http_slash = consts::http_slash()
        , accept   = consts::accept()
        , accept_mime = consts::default_accept_mime()
        , accept_encoding = consts::accept_encoding()
        , default_accept_encoding = consts::default_accept_encoding()
        , default_user_agent = consts::default_user_agent()
        , user_agent = consts::user_agent()
        , crlf = consts::crlf()
        , host_const = consts::host()
        , connection = consts::connection()
        , close = consts::close()
        ;
    boost::copy(method, oi);
    *oi = consts::space_char();
    {
      std::string path_ = path(request);
      if (path_.empty() || path_[0] != consts::slash_char()) 
          *oi = consts::slash_char();
      boost::copy(path_, oi);
    }
    {
      std::string query_ = query(request);
      if (!query_.empty()) {
          *oi = consts::question_mark_char();
          boost::copy(query_, oi);
      }
    }
    {
      std::string anchor_ = anchor(request);
      if (!anchor_.empty()) {
        *oi = consts::hash_char();
        boost::copy(anchor_, oi);
      }
    }
    *oi = consts::space_char();
    boost::copy(http_slash, oi);
    string_type version_major_str = boost::lexical_cast<string_type>(version_major),
                version_minor_str = boost::lexical_cast<string_type>(version_minor);
    boost::copy(version_major_str, oi);
    *oi = consts::dot_char();
    boost::copy(version_minor_str, oi);
    boost::copy(crlf, oi);
    boost::copy(host_const, oi);
    *oi = consts::colon_char();
    *oi = consts::space_char();
    {
      std::string host_ = host(request);
      boost::copy(host_, oi);
    }
    boost::optional<boost::uint16_t> port_ = port(request);
    if (port_) {
        string_type port_str = boost::lexical_cast<string_type>(*port_);
        *oi = consts::colon_char();
        boost::copy(port_str, oi);
    }
    boost::copy(crlf, oi);
    boost::copy(accept, oi);
    *oi = consts::colon_char();
    *oi = consts::space_char();
    boost::copy(accept_mime, oi);
    boost::copy(crlf, oi);
    if (version_major == 1u && version_minor == 1u) {
        boost::copy(accept_encoding, oi);
        *oi = consts::colon_char();
        *oi = consts::space_char();
        boost::copy(default_accept_encoding, oi);
        boost::copy(crlf, oi);
    }
    typedef headers_wrapper::container_type headers_container;
    typedef headers_container::const_iterator headers_iterator;
    headers_container const & request_headers = network::headers(request);
    headers_iterator iterator = boost::begin(request_headers),
                     end = boost::end(request_headers);
    bool has_user_agent = false;
    for (; iterator != end; ++iterator) {
        string_type header_name = name(*iterator),
                    header_value = value(*iterator);
        boost::copy(header_name, oi);
        *oi = consts::colon_char();
        *oi = consts::space_char();
        boost::copy(header_value, oi);
        boost::copy(crlf, oi);
        boost::to_lower(header_name);
        has_user_agent = has_user_agent || header_name == "user-agent";
    }
    if (!has_user_agent) {
      boost::copy(user_agent, oi);
      *oi = consts::colon_char();
      *oi = consts::space_char();
      boost::copy(default_user_agent, oi);
      boost::copy(crlf, oi);
    }
    boost::copy(crlf, oi);
    auto body_data = network::body(request);
    return std::copy(body_data.begin(), body_data.end(), oi);
}
    
}  // namespace http
}  // namespace network
    
#endif /* NETWORK_PROTOCOL_HTTP_ALGORITHMS_LINEARIZE_HPP_20101028 */
