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

            boost::asio::io_service * service_ptr;
            boost::asio::io_service & service_;
            resolver_type resolver_;
            optional<string_type> certificate_file, verify_path;

            sync_client(bool cache_resolved, bool follow_redirect
                , optional<string_type> const & certificate_file = optional<string_type>()
                , optional<string_type> const & verify_path = optional<string_type>()
            )
                : connection_base(cache_resolved, follow_redirect),
                service_ptr(new boost::asio::io_service),
                service_(*service_ptr),
                resolver_(service_)
                , certificate_file(certificate_file)
                , verify_path(verify_path)
            {}

            sync_client(bool cache_resolved, bool follow_redirect, boost::asio::io_service & service
                , optional<string_type> const & certificate_file = optional<string_type>()
                , optional<string_type> const & verify_path = optional<string_type>()
            )
                : connection_base(cache_resolved, follow_redirect),
                service_ptr(0),
                service_(service),
                resolver_(service_)
                , certificate_file(certificate_file)
                , verify_path(verify_path)
            {}

            ~sync_client() {
                delete service_ptr;
            }

            basic_response<Tag> const request_skeleton(basic_request<Tag> const & request_, string_type method, bool get_body) {
                typename connection_base::connection_ptr connection_;
                connection_ = connection_base::get_connection(resolver_, request_, certificate_file, verify_path);
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
        typedef typename base_type::string_type string_type;

        basic_client_impl(bool cache_resolved, bool follow_redirect, optional<string_type> const & certificate_filename, optional<string_type> const & verify_path)
            : base_type(cache_resolved, follow_redirect, certificate_filename, verify_path)
        {}

        basic_client_impl(bool cache_resolved, bool follow_redirect, boost::asio::io_service & service, optional<string_type> const & certificate_filename, optional<string_type> const & verify_path)
            : base_type(cache_resolved, follow_redirect, service, certificate_filename, verify_path)
        {}

        ~basic_client_impl()
        {}
    };

} // namespace http

} // namespace network

} // namespace boost

#endif // BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_PIMPL_HPP_20100623
