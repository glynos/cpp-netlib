
//          Copyright Dean Michael Berris 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef __NETWORK_MESSAGE_HPP__
#define __NETWORK_MESSAGE_HPP__

#include <map>
#include <string>

// forward declarations
namespace boost { namespace network {
    template <class tag>
        class basic_message;
}; // namespace network

}; // namespace boost

#include <boost/network/detail/directive_base.hpp>
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

    struct tags {
        struct default_ { };
    };

    /** The common message type.
     */
    template <class tag=tags::default_>
    class basic_message {
        public:
        typedef std::multimap<std::string, std::string> headers_container_type;

        headers_container_type & headers() const {
            return _headers;
        };

        std::string & body() const {
            return _body;
        };
        
        std::string & source() const {
            return _source;
        };

        std::string & destination() const {
            return _destination;
        };

        private:
        
        friend struct detail::directive_base<tag> ;
        friend struct detail::wrapper_base<tag> ;

        mutable headers_container_type _headers;
        mutable std::string _body;
        mutable std::string _source;
        mutable std::string _destination;
    };

    typedef basic_message<> message; // default message type

}; // namespace network
}; // namespace boost

#include <boost/network/message/directives.hpp>
    // pull in directives header file

#include <boost/network/message/wrappers.hpp>
    // pull in wrappers header file

#endif // __NETWORK_MESSAGE_HPP__

