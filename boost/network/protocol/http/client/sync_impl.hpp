#ifndef BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_SYNC_IMPL_HPP_20100623
#define BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_SYNC_IMPL_HPP_20100623

// Copyright Dean Michael Berris 2010.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

namespace boost { namespace network { namespace http {

    template <class Tag, unsigned version_major, unsigned version_minor>
    struct basic_client_impl;

    namespace impl {
        template <class Tag, unsigned version_major, unsigned version_minor>
        struct sync_client :
            connection_policy<Tag,version_major,version_minor>::type
        {
            typedef 
                typename connection_policy<Tag,version_major,version_minor>::type
                connection_base;

            sync_client(bool cache_resolved, bool follow_redirect)
                : connection_base(cache_resolved, follow_redirect)
            {}

            ~sync_client()
            {
                connection_base::cleanup();
            }
            
            basic_response<Tag> const request_skeleton(basic_request<Tag> const & request_, string_type method, bool get_body) {
                typename connection_base::connection_ptr connection_;
                connection_ = connection_base::get_connection(resolver_, request_);
                return connection_->send_request(method, request_, get_body);
            }

        };
    } // namespace impl

} // namespace http

} // namespace network

} // namespace boost

#endif // BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_SYNC_IMPL_HPP_20100623
