//          Copyright Dean Michael Berris 2007.
//          Copyright Michael Dickey 2008.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_NETWORK_PROTOCOL_HTTP_RESPONSE_HPP
#define BOOST_NETWORK_PROTOCOL_HTTP_RESPONSE_HPP

#include <boost/cstdint.hpp>

#include <boost/network/protocol/http/response_base.hpp>
#include <boost/network/protocol/http/message/directives/status_message.hpp>
#include <boost/network/protocol/http/message/directives/version.hpp>
#include <boost/network/protocol/http/message/directives/status.hpp>
#include <boost/network/protocol/http/message/directives/uri.hpp>

#include <boost/network/protocol/http/message/modifiers/uri.hpp>
#include <boost/network/protocol/http/message/modifiers/version.hpp>
#include <boost/network/protocol/http/message/modifiers/status.hpp>
#include <boost/network/protocol/http/message/modifiers/status_message.hpp>

#include <boost/network/protocol/http/message/wrappers/version.hpp>
#include <boost/network/protocol/http/message/wrappers/status.hpp>
#include <boost/network/protocol/http/message/wrappers/status_message.hpp>
#include <boost/network/protocol/http/message/wrappers/ready.hpp>

#include <boost/network/protocol/http/response_concept.hpp>

namespace boost { namespace network { namespace http {

struct response : response_base {
  // FIXME implement all these!

  // From message_base...
  // Mutators
  virtual void set_destination(std::string const & destination);
  virtual void set_source(std::string const & source);
  virtual void append_header(std::string const & name,
                             std::string const & value);
  virtual void remove_headers(std::string const & name);
  virtual void remove_headers();
  virtual void set_body(std::string const & body);
  virtual void append_body(std::string const & data);

  // Retrievers
  virtual void get_destination(std::string & destination);
  virtual void get_source(std::string & source);
  virtual void get_headers(function<void(std::string const &, std::string const &)> inserter);
  virtual void get_headers(std::string const & name, function<void(std::string const &, std::string const &)> inserter);
  virtual void get_headers(function<bool(std::string const &, std::string const &)> predicate, function<void(std::string const &, std::string const &)> inserter);
  virtual void get_body(std::string & body);
  virtual void get_body(function<void(iterator_range<char const *>)> chunk_reader, size_t size);

  // From response_base...
  virtual void set_status(std::string const & new_status);
  virtual void set_status_message(std::string const & new_status_message);
  virtual void set_version(std::string const & new_version);
  virtual ~response();
};

} // namespace http

} // namespace network

} // namespace boost

#include <boost/network/protocol/http/impl/response.ipp>

namespace boost { namespace network { namespace http {

    template <class Directive>
    response & operator<<(
        response & message,
        Directive const & directive
        )
    {
        directive(message);
        return message;
    }

} // namespace http

} // namespace network

} // namespace boost

#endif // BOOST_NETWORK_PROTOCOL_HTTP_RESPONSE_HPP
