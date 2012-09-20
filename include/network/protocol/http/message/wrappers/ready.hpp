// Copyright 2010 Dean Michael Berris. 
// Copyright 2012 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef NETWORK_PROTOCOL_HTTP_MESSAGE_WRAPPERS_READY_HPP_20100618
#define NETWORK_PROTOCOL_HTTP_MESSAGE_WRAPPERS_READY_HPP_20100618

#include <network/detail/wrapper_base.hpp>

namespace network { namespace http {

    template <class Tag>
    struct async_message;

    namespace impl {
        template <class Tag>
        struct ready_wrapper : boost::network::detail::wrapper_base_const<Tag, async_message<Tag> > {
            typedef boost::network::detail::wrapper_base_const<Tag, async_message<Tag> >
                wrapper_base;
            explicit ready_wrapper(async_message<Tag> const & message)
            : wrapper_base(message) {}
            operator bool () {
                return 
                    wrapper_base::_message.version_.is_ready()
                    && wrapper_base::_message.status_.is_ready()
                    && wrapper_base::_message.status_message_.is_ready()
                    && wrapper_base::_message.headers_.is_ready()
                    && wrapper_base::_message.body_.is_ready();
            }
        };
    } // namespace impl

    template <class Tag>
    inline bool ready(async_message<Tag> const & message) {
        return impl::ready_wrapper<Tag>(message);
    }
    
}  // namespace http
}  // namespace network

#endif /* NETWORK_PROTOCOL_HTTP_MESSAGE_WRAPPERS_READY_HPP_20100618 */
