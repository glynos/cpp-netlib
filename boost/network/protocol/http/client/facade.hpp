#ifndef BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_FACADE_HPP_20100623
#define BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_FACADE_HPP_20100623

// Copyright Dean Michael Berris 2010.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/protocol/http/request.hpp>
#include <boost/network/protocol/http/response.hpp>
#include <boost/network/support/sync_only.hpp>

namespace boost { namespace network { namespace http {

    template <class Tag>
    struct basic_request;

    template <class Tag>
    struct basic_response;

    template <class Tag, class Derived>
    struct basic_client_facade {

        typedef typename string<Tag>::type string_type;
        typedef basic_request<typename sync_only<Tag>::type> request;
        typedef basic_response<Tag> response;

        basic_client_facade()
        {}

        response const head (request const & request_) {
            return static_cast<Derived*>(this)->request_skeleton(request_, "HEAD", false);
        }

        response const get (request const & request_) {
            return static_cast<Derived*>(this)->request_skeleton(request_, "GET", true);
        }

        response const post (request const & request_) {
            return static_cast<Derived*>(this)->request_skeleton(request_, "POST", true);
        }

        response const post (request request_, string_type const & content_type, string_type const & body_) {
            request_ << ::boost::network::body(body_)
                << header("Content-Type", content_type)
                << header("Content-Length", boost::lexical_cast<string_type>(body_.size()));
            return post(request_);
        }

        response const post (request const & request_, string_type const & body_) {
            return post(request_, "x-application/octet-stream", body_);
        }

        response const put (request const & request_) {
            return static_cast<Derived*>(this)->request_skeleton(request_, "PUT", true);
        }

        response const put (request const & request_, string_type const & body_) {
            return put(request_, "x-application/octet-stream", body_);
        }

        response const put (request request_, string_type const & content_type, string_type const & body_) {
            request_ << ::boost::network::body(body_)
                << header("Content-Type", content_type)
                << header("Content-Length", boost::lexical_cast<string_type>(body_.size()));
            return put(request_);
        }

        response const delete_ (request const & request_) {
            return static_cast<Derived*>(this)->request_skeleton(request_, "DELETE", true);
        }

        void clear_resolved_cache() {
            static_cast<Derived*>(this)->clear_resolved_cache();
        }

    };

} // namespace http

} // namespace network

} // namespace boost

#endif // BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_FACADE_HPP_20100623
