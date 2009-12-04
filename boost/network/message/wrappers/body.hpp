
//          Copyright Dean Michael Berris 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef __NETWORK_MESSAGE_WRAPPERS_BODY_HPP__
#define __NETWORK_MESSAGE_WRAPPERS_BODY_HPP__


#include <boost/network/traits/string.hpp>


namespace boost { namespace network {

    namespace impl {
        template <class Tag>
            struct body_wrapper : public detail::wrapper_base<Tag> {
                typedef Tag tag;
                typedef basic_message<tag> message_type;
                typedef typename string<Tag>::type string_type;
                
                explicit body_wrapper(basic_message<tag> & message_)
                    : detail::wrapper_base<tag>(message_)
                { };

                operator string_type () const {
                    return string_type(detail::wrapper_base<Tag>::_message.body());
                };
            };
    } // namespace impl

    template <class Tag>
        inline typename string<Tag>::type
        body(basic_message<Tag> & message_) {
            return impl::body_wrapper<Tag>(message_);
        }

} // namespace network

} // namespace boost

#endif // __NETWORK_MESSAGE_WRAPPERS_BODY_HPP__

