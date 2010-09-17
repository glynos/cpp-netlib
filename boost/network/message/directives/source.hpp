
//          Copyright Dean Michael Berris 2007-2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef __NETWORK_MESSAGE_DIRECTIVES_SOURCE_HPP__
#define __NETWORK_MESSAGE_DIRECTIVES_SOURCE_HPP__

#include <boost/network/message/directives/detail/string_directive.hpp>
#include <boost/network/message/directives/detail/string_value.hpp>

namespace boost { namespace network {

    namespace impl {

        struct source_directive_base {

            template <class Message>
            struct string_visitor : boost::static_visitor<> {
                Message const & message_;
                string_visitor(Message const & message)
                    : message_(message) {}
                string_visitor(string_visitor const & other)
                    : message_(other.message_) {}
                void operator()(typename detail::string_value<typename Message::tag>::type const & source) const {
                    message_.source(source);
                }
                template <class T> void operator()(T const &) const {
                    // FIXME -- fail here!
                }
            };

        };

        typedef detail::string_directive<source_directive_base> source_directive;

    } // namespace impl

    template <class Input>
    inline impl::source_directive const source(Input const & input) {
        return impl::source_directive(input);
    }

} // namespace network
} // namespace boost

#endif // __NETWORK_MESSAGE_DIRECTIVES_SOURCE_HPP__

