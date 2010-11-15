
//          Copyright Dean Michael Berris 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef __NETWORK_MESSAGE_WRAPPERS_BODY_HPP__
#define __NETWORK_MESSAGE_WRAPPERS_BODY_HPP__

#include <boost/network/traits/string.hpp>
#include <boost/network/detail/wrapper_base.hpp>

namespace boost { namespace network {

    namespace impl {
        template <class Tag>
            struct body_wrapper : public detail::wrapper_base<Tag, basic_message<Tag> > {
                typedef basic_message<Tag> message_type;
                typedef typename string<Tag>::type string_type;
                typedef detail::wrapper_base<Tag, basic_message<Tag> > wrapper_base;
                
                explicit body_wrapper(basic_message<Tag> & message_)
                    : wrapper_base(message_)
                { };

                operator string_type () const {
                    return string_type(wrapper_base::_message.body());
                };

                std::size_t size() const {
                    return wrapper_base::_message.body().size();
                }
            };

        template <class Tag>
            struct body_wrapper_const : public detail::wrapper_base_const<Tag, basic_message<Tag> > {
                typedef basic_message<Tag> message_type;
                typedef typename string<Tag>::type string_type;
                typedef detail::wrapper_base_const<Tag, basic_message<Tag> > wrapper_base;

                explicit body_wrapper_const(basic_message<Tag> const & message_)
                    : wrapper_base(message_)
                {};

                operator string_type () const {
                    return string_type(wrapper_base::_message.body());
                }

                std::size_t size() const {
                    return wrapper_base::_message.body().size();
                }
            };

    } // namespace impl

    template <class Tag>
        inline impl::body_wrapper<Tag> const
        body(basic_message<Tag> & message_) {
            return impl::body_wrapper<Tag>(message_);
        }

    template <class Tag>
        inline impl::body_wrapper_const<Tag> const
        body(basic_message<Tag> const & message_) {
            return impl::body_wrapper_const<Tag>(message_);
        }

} // namespace network

} // namespace boost

#endif // __NETWORK_MESSAGE_WRAPPERS_BODY_HPP__

