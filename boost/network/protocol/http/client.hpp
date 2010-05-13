#ifndef BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_20091215
#define BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_20091215

//          Copyright Dean Michael Berris 2007-2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/version.hpp>
#include <boost/network/traits/ostringstream.hpp>
#include <boost/network/protocol/http/message.hpp>
#include <boost/network/protocol/http/response.hpp>
#include <boost/network/protocol/http/request.hpp>
#include <boost/network/protocol/http/traits/connection_policy.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/foreach.hpp>
#include <ostream>
#include <istream>
#include <string>
#include <stdexcept>
#include <map>

namespace boost { namespace network { namespace http {

    template <class Tag, unsigned version_major, unsigned version_minor>
    struct basic_client : connection_policy<Tag, version_major, version_minor>::type {
        private:
        typedef typename string<Tag>::type string_type;

        public:
        typedef basic_request<Tag> request;
        typedef basic_response<Tag> response;

        struct cache_resolved_type { };

        static cache_resolved_type cache_resolved() {
            return cache_resolved_type();
        };

        struct follow_redirect_type { };

        static follow_redirect_type follow_redirects() {
            return follow_redirect_type();
        };

        static follow_redirect_type follow_redirect() {
            return follow_redirect_type();
        };

        // Constructors
        // =================================================================
        basic_client()
        : connection_base(false, false), service_(), resolver_(service_)
        {}

        explicit basic_client(cache_resolved_type (*)())
        : connection_base(true, false), service_(), resolver_(service_)
        {}

        explicit basic_client(follow_redirect_type (*)())
        : connection_base(false, true), service_(), resolver_(service_)
        {}

        basic_client(cache_resolved_type (*)(), follow_redirect_type (*)())
        : connection_base(true, true), service_(), resolver_(service_)
        {}

        //
        // =================================================================

        ~basic_client() {
            connection_base::cleanup();
        }

        void clear_resolved_cache() {
            connection_base::endpoint_cache_.clear();
        }

        response const head (request const & request_) {
            return sync_request_skeleton(request_, "HEAD", false);
        };

        response const get (request const & request_) {
            return sync_request_skeleton(request_, "GET", true);
        };

        response const post (request const & request_) {
            return sync_request_skeleton(request_, "POST", true);
        };

        response const post (request request_, string_type const & content_type, string_type const & body_) {
            request_ << body(body_)
                << header("Content-Type", content_type)
                << header("Content-Length", boost::lexical_cast<string_type>(body_.size()));
            return post(request_);
        };

        response const post (request const & request_, string_type const & body_) {
            return post(request_, "x-application/octet-stream", body_);
        };

        response const put (request const & request_) {
            return sync_request_skeleton(request_, "PUT", true);
        };

        response const put (request const & request_, string_type const & body_) {
            return put(request_, "x-application/octet-stream", body_);
        };

        response const put (request request_, string_type const & content_type, string_type const & body_) {
            request_ << body(body_)
                << header("Content-Type", content_type)
                << header("Content-Length", boost::lexical_cast<string_type>(body_.size()));
            return put(request_);
        };

        response const delete_ (request const & request_) {
            return sync_request_skeleton(request_, "DELETE", true);
        };

        private:

        typedef typename connection_policy<Tag, version_major, version_minor>::type connection_base;
        boost::asio::io_service service_;
        typename connection_base::resolver_type resolver_;

        basic_response<Tag> const sync_request_skeleton(basic_request<Tag> const & request_, string_type method, bool get_body) {
            typename connection_base::connection_ptr connection_;
            connection_ = connection_base::get_connection(resolver_, request_);
            return connection_->send_request(method, request_, get_body);
        }

    };

    typedef basic_client<tags::http_default_8bit_udp_resolve, 1, 0> client;

} // namespace http

} // namespace network

} // namespace boost

#endif // BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_20091215
