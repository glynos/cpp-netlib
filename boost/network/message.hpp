//          Copyright Dean Michael Berris 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef __NETWORK_MESSAGE_HPP__
#define __NETWORK_MESSAGE_HPP__

#include "boost/network/message_fwd.hpp"
#include <boost/network/traits/string.hpp>
#include <boost/network/traits/ostringstream.hpp>
#include <boost/network/traits/headers_container.hpp>
#include <boost/network/detail/directive_base.hpp>
#include <boost/network/detail/wrapper_base.hpp>
#include <boost/network/message/directives.hpp>
#include <boost/network/message/wrappers.hpp>
#include <boost/network/message/transformers.hpp>

#include <boost/network/message/message_concept.hpp>

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

        typedef typename headers_container<Tag>::type headers_container_type;
        typedef typename string<Tag>::type string_type;

        basic_message()
            : _headers(), _body(), _source(), _destination()
        { }

        basic_message(const basic_message & other)
            : _headers(other._headers), _body(other._body), _source(other._source), _destination(other._destination)
        { }

        basic_message & operator=(basic_message<Tag> rhs) {
            rhs.swap(*this);
            return *this;
        }

        void swap(basic_message<Tag> & other) {
            other._headers.swap(_headers);
            other._body.swap(_body);
            other._source.swap(_source);
            other._destination.swap(_destination);
        }

        headers_container_type & headers() {
            return _headers;
        }

        headers_container_type headers() const {
            return _headers;
        }

        string_type & body() {
            return _body;
        }

        string_type body() const {
            return _body;
        }
        
        string_type & source() {
            return _source;
        }

        string_type source() const {
            return _source;
        }

        string_type & destination() {
            return _destination;
        }

        string_type destination() const {
            return _destination;
        }

        private:
        
        friend struct detail::directive_base<Tag> ;
        friend struct detail::wrapper_base<Tag> ;

        headers_container_type _headers;
        string_type _body;
        string_type _source;
        string_type _destination;
    };

    template <class Tag>
    inline void swap(basic_message<Tag> & left, basic_message<Tag> & right) {
        // swap for ADL
        left.swap(right);
    }

BOOST_CONCEPT_ASSERT((Message<basic_message<boost::network::tags::default_string> >));
BOOST_CONCEPT_ASSERT((Message<basic_message<boost::network::tags::default_wstring> >));

} // namespace network
} // namespace boost

#endif // __NETWORK_MESSAGE_HPP__

