#ifndef BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_MODIFIERS_CLEAR_HEADER_HPP_20101128
#define BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_MODIFIERS_CLEAR_HEADER_HPP_20101128

#include <boost/network/protocol/http/support/client_or_server.hpp>
#include <boost/network/support/is_async.hpp>
#include <boost/thread/future.hpp>

namespace boost { namespace network { namespace http {

    namespace impl {

        template <class Tag>
        inline typename enable_if<
            is_client<Tag>
            , void
            >::type
        clear_headers(basic_request<Tag> & request) {
            request.headers(typename basic_request<Tag>::headers_container_type());
        }

    } /* impl */

    template <class Tag>
    inline void clear_headers_impl(basic_request<Tag> & request, tags::client) {
        impl::clear_headers(request);
    }

    template <class Tag>
    inline void clear_headers_impl(basic_request<Tag> & request, tags::server) {
        typedef typename basic_request<Tag>::headers_container_type headers_container;
        headers_container().swap(request.headers);
    }

    template <class Tag>
    inline void clear_headers(basic_request<Tag> & request) {
        clear_headers_impl(request, typename client_or_server<Tag>::type());
    }
    
} /* http */
    
} /* network */
    
} /* boost */

#endif /* BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_MODIFIERS_CLEAR_HEADER_HPP_20101128 */
