
//          Copyright Dean Michael Berris 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef __NETWORK_DETAIL_WRAPPER_BASE_HPP__
#define __NETWORK_DETAIL_WRAPPER_BASE_HPP__

namespace boost { namespace network { namespace detail {

    template <class Tag>
        struct wrapper_base {
            typedef Tag tag;
            explicit wrapper_base(basic_message<tag> & message_)
                : _message(message_)
            { };

            protected:
                virtual ~wrapper_base() 
                { }; // for extending only

                mutable basic_message<tag> & _message;
        };

} // namespace detail

} // namespace network

} // namespace boost

#endif // __NETWORK_DETAIL_WRAPPER_BASE_HPP__

