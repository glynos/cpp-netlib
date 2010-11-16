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
#include <boost/network/protocol/http/client/async_impl.hpp>

namespace boost { namespace network { namespace http {

    template <class Tag, unsigned version_major, unsigned version_minor>
    struct basic_client_impl;

    namespace impl {

        template <class Tag, unsigned version_major, unsigned version_minor>
        struct async_client;
        
        template <class Tag, unsigned version_major, unsigned version_minor>
        struct sync_client :
            connection_policy<Tag, version_major, version_minor>::type
        {
            typedef typename string<Tag>::type string_type;
            typedef typename connection_policy<Tag,version_major,version_minor>::type connection_base;
            typedef typename resolver<Tag>::type resolver_type;
            friend struct basic_client_impl<Tag,version_major,version_minor>;

            std::auto_ptr<boost::asio::io_service> service_ptr;
            boost::asio::io_service & service_;
            resolver_type resolver_;

            sync_client(bool cache_resolved, bool follow_redirect)
                : connection_base(cache_resolved, follow_redirect),
                service_ptr(new boost::asio::io_service),
                service_(*service_ptr),
                resolver_(service_)
            {}

            sync_client(bool cache_resolved, bool follow_redirect, boost::asio::io_service & service)
                : connection_base(cache_resolved, follow_redirect),
                service_ptr(),
                service_(service),
                resolver_(service_)
            {}

            ~sync_client() {}

            basic_response<Tag> const request_skeleton(basic_request<Tag> const & request_, string_type method, bool get_body) {
                typename connection_base::connection_ptr connection_;
                connection_ = connection_base::get_connection(resolver_, request_);
                return connection_->send_request(method, request_, get_body);
            }

        };

        template <class Tag, unsigned version_major, unsigned version_minor>
        struct client_base
            : mpl::if_<
                typename is_async<Tag>::type,
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
        
        typedef typename impl::client_base<Tag,version_major,version_minor>::type base_type;
        basic_client_impl(bool cache_resolved, bool follow_redirect)
            : base_type(cache_resolved, follow_redirect)
        {}

        basic_client_impl(bool cache_resolved, bool follow_redirect, boost::asio::io_service & service)
            : base_type(cache_resolved, follow_redirect, service)
        {}

        ~basic_client_impl()
        {}
    };

} // namespace http

} // namespace network

} // namespace boost

#endif // BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_PIMPL_HPP_20100623
