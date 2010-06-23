#ifndef BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_PIMPL_HPP_20100623
#define BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_PIMPL_HPP_20100623

// Copyright Dean Michael Berris 2010.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/support/is_async.hpp>
#include <boost/network/support/is_sync.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/if.hpp>
#include <boost/static_assert.hpp>

#include <boost/network/protocol/http/traits/connection_policy.hpp>

namespace boost { namespace network { namespace http {

    template <class Tag, unsigned version_major, unsigned version_minor>
    struct basic_client_impl;

    namespace impl {

        template <class Tag, unsigned version_major, unsigned version_minor>
        struct async_client;
        
        template <class Tag, unsigned version_major, unsigned version_minor>
        struct sync_client
        {
        protected:
            friend struct basic_client_impl<Tag,version_major,version_minor>;

            basic_response<Tag> const request_skeleton(basic_request<Tag> const & request_, string_type method, bool get_body) {
                typename connection_base::connection_ptr connection_;
                connection_ = connection_base::get_connection(resolver_, request_);
                return connection_->send_request(method, request_, get_body);
            }

        };

        template <class Tag, unsigned version_major, unsigned version_minor>
        struct client_base
            : mpl::if_<
                is_async<Tag>,
                async_client<Tag,version_major,version_minor>,
                sync_client<Tag,version_major,version_minor>
            >
        {};

    } // namespace impl

    template <class Tag, unsigned version_major, unsigned version_minor>
    struct basic_client;

    template <class Tag, unsigned version_major, unsigned version_minor>
    struct basic_client_impl
        : impl::client_base<Tag,version_major,version_minor>::type
    {
        BOOST_STATIC_ASSERT((
            mpl::not_<
                mpl::and_<
                    is_async<Tag>,
                    is_sync<Tag>
                >
            >::value
            ));
        
    private:
        typedef typename impl::client_base<Tag,version_major,version_minor>::type base_type;
        basic_client_impl(bool cache_resolved, bool follow_redirect)
            : base_type(cache_resolved, follow_redirect)
        {}

        ~basic_client_impl()
        {}
    };

    
    };

} // namespace http

} // namespace network

} // namespace boost

#endif // BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_PIMPL_HPP_20100623
