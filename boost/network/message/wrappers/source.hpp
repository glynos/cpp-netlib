
//          Copyright Dean Michael Berris 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef __NETWORK_MESSAGE_WRAPPERS_SOURCE_HPP__
#define __NETWORK_MESSAGE_WRAPPERS_SOURCE_HPP__

namespace boost { namespace network {

    namespace impl {
        template <class Tag>
            struct source_wrapper : public detail::wrapper_base<Tag> {
                typedef Tag tag;
                typedef basic_message<tag> message_type;

                explicit source_wrapper(basic_message<tag> & message_)
                    : detail::wrapper_base<tag>(message_)
                { };

                operator std::string () const {
                    return std::string(detail::wrapper_base<tag>::_message.source());
                };
            };
    } // namespace impl

    template <class Tag>
        inline std::string
        source(basic_message<Tag> & message_) {
            return impl::source_wrapper<Tag>(message_);
        }

} // namespace network

} // namespace boost

#endif // __NETWORK_MESSAGE_WRAPPERS_SOURCE_HPP__


