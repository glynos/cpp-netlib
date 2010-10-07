
//          Copyright Dean Michael Berris 2007-2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef __NETWORK_MESSAGE_DIRECTIVES_BODY_HPP__
#define __NETWORK_MESSAGE_DIRECTIVES_BODY_HPP__

#include <boost/network/message/directives/detail/string_directive.hpp>
#include <boost/network/message/directives/detail/string_value.hpp>

namespace boost { namespace network {

    namespace impl {

        struct body_directive_base {

            template <class Message>
            struct string_visitor : boost::static_visitor<> {
                Message const & message_;
                string_visitor(Message const & message)
                    : message_(message) {}
                void operator()(typename detail::string_value<typename Message::tag>::type const & body) const {
                    message_.body(body);
                }
                template <class T> void operator()(T const &) const {
                    // FIXME -- fail here
                }
            };

        };

        typedef detail::string_directive<body_directive_base> body_directive;

    } // namespace impl


    inline impl::body_directive const body(string<tags::default_string>::type const & input) {
        return impl::body_directive(input);
    }

    inline impl::body_directive const body(string<tags::default_wstring>::type const & input) {
        return impl::body_directive(input);
    }

    template <class InputType>
    inline impl::body_directive const body(boost::shared_future<InputType> const & input) {
        return impl::body_directive(input);
    }

} // namespace network
} // namespace boost

#endif // __NETWORK_MESSAGE_DIRECTIVES_BODY_HPP__

