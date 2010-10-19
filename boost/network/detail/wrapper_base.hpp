
//          Copyright Dean Michael Berris 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef __NETWORK_DETAIL_WRAPPER_BASE_HPP__
#define __NETWORK_DETAIL_WRAPPER_BASE_HPP__

namespace boost { namespace network {

    template <class Tag>
    struct basic_message;
    
    namespace detail {

        template <class Tag>
            struct wrapper_base {
                explicit wrapper_base(basic_message<Tag> & message_)
                    : _message(message_)
                {};

                protected:
                    ~wrapper_base() {}; // for extending only

                    basic_message<Tag> & _message;
            };

        template <class Tag>
            struct wrapper_base_const {
                explicit wrapper_base_const(basic_message<Tag> const & message_)
                    : _message(message_)
                {}

                protected:
                    ~wrapper_base_const() {}; // for extending only

                    basic_message<Tag> const & _message;
            };

    } // namespace detail

} // namespace network

} // namespace boost

#endif // __NETWORK_DETAIL_WRAPPER_BASE_HPP__

