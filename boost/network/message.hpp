//          Copyright Dean Michael Berris 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef __NETWORK_MESSAGE_HPP__
#define __NETWORK_MESSAGE_HPP__

#include <boost/network/message_fwd.hpp>
#include <boost/network/detail/directive_base.hpp>
#include <boost/network/detail/wrapper_base.hpp>
#include <boost/network/message/directives.hpp>
#include <boost/network/message/wrappers.hpp>
#include <boost/network/message/transformers.hpp>

#include <boost/network/message/modifiers/add_header.hpp>
#include <boost/network/message/modifiers/remove_header.hpp>
#include <boost/network/message/modifiers/clear_headers.hpp>
#include <boost/network/message/modifiers/source.hpp>
#include <boost/network/message/modifiers/destination.hpp>
#include <boost/network/message/modifiers/body.hpp>

#ifdef BOOST_NETWORK_DEBUG
#include <boost/network/message/message_concept.hpp>
#endif

/** message.hpp
 *
 * This header file implements the common message type which
 * all networking implementations under the boost::network
 * namespace. The common message type allows for easy message
 * construction and manipulation suited for networked
 * application development.
 */
namespace boost { namespace network {

    /** The common message type.
     */
    struct message : message_base {
      // Nested types
      typedef iterator_range<
        shared_container_iterator<std::multimap<std::string, std::string> > >
        headers_range;

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
      virtual void get_body(std::string const & body);
      virtual void get_body(function<void(iterator_range<char const *>)> chunk_reader, size_t size);
      void swap(message & other);
      virtual ~message();
    };

    inline void swap(message & left, message & right) {
        left.swap(right);
    }

    template <class Directive>
    message_base & operator<< (message_base & msg, Directive directive) {
      directive(msg);
      return msg;
    }
    
} // namespace network
} // namespace boost

#endif // __NETWORK_MESSAGE_HPP__

