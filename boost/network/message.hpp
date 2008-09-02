
//          Copyright Dean Michael Berris 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef __NETWORK_MESSAGE_HPP__
#define __NETWORK_MESSAGE_HPP__

#include <map>
#include <string>


// include message declaration
#include "boost/network/message_fwd.hpp"

// include traits implementation
#include <boost/network/message/traits.hpp>

// include directives base
#include <boost/network/detail/directive_base.hpp>

// include wrappers base
#include <boost/network/detail/wrapper_base.hpp>

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
    template <class Tag>
    class basic_message {
        public:

        typedef Tag tag;

        typedef typename headers_container<tag>::type headers_container_type;
		typedef typename string<tag>::type string_type;

        basic_message()
            : _headers(), _body(), _source(), _destination()
        { };

        basic_message(const basic_message & other)
            : _headers(other._headers), _body(other._body), _source(other._source), _destination(other._destination)
        { };

        basic_message & operator=(basic_message<tag> rhs) {
            rhs.swap(*this);
            return *this;
        };

        void swap(basic_message<tag> & other) {
            other._headers.swap(_headers);
            other._body.swap(_body);
            other._source.swap(_source);
            other._destination.swap(_destination);
        };

        headers_container_type & headers() {
            return _headers;
        };

        string_type & body() {
            return _body;
        };
        
        string_type & source() {
            return _source;
        };

        string_type & destination() {
            return _destination;
        };

        private:
        
        friend struct detail::directive_base<tag> ;
        friend struct detail::wrapper_base<tag> ;

        headers_container_type _headers;
		string_type _body;
		string_type _source;
        string_type _destination;
    };

    template <class Tag>
    void swap(basic_message<Tag> & left, basic_message<Tag> & right) {
        // swap for ADL
        left.swap(right);
    }

} // namespace network

} // namespace boost

#include <boost/network/message/directives.hpp>
    // pull in directives header file

#include <boost/network/message/wrappers.hpp>
    // pull in wrappers header file

#include <boost/network/message/transformers.hpp>
    // pull in transformer header file

#endif // __NETWORK_MESSAGE_HPP__

