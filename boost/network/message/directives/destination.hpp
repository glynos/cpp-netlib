
//          Copyright Dean Michael Berris 2007-2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef __NETWORK_MESSAGE_DIRECTIVES_DESTINATION_HPP__
#define __NETWORK_MESSAGE_DIRECTIVES_DESTINATION_HPP__

#include <boost/network/message/directives/detail/string_directive.hpp>
#include <boost/network/message/directives/detail/string_value.hpp>

namespace boost { namespace network {

    namespace impl {

        struct destination_directive_base {

            template <class Message>
            struct string_visitor : boost::static_visitor<> {
                Message const & message_;
                string_visitor(Message const & message)
                    : message_(message) {}
                void operator()(typename detail::string_value<typename Message::tag>::type const & destination) const {
                    message_.destination(destination);
                }
                template <class T> void operator() (T const &) const {
                    // FIXME -- fail here!
                }
            };

        };
        
        typedef detail::string_directive<destination_directive_base> destination_directive;

    } // namespace impl

    template <class T>
    inline impl::destination_directive const
    destination(T const & destination_) {
        return impl::destination_directive(destination_);
    }

} // namespace network

} // namespace boost
 

#endif // __NETWORK_MESSAGE_DIRECTIVES_DESTINATION_HPP__
