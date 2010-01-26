
//          Copyright Dean Michael Berris 2007,2009.
//          Copyright Michael Dickey 2008.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef __NETWORK_PROTOCOL_HTTP_REQUEST_IMPL_20070908_1_HPP__
#define __NETWORK_PROTOCOL_HTTP_REQUEST_IMPL_20070908_1_HPP__

#include <boost/network/protocol/http/message.hpp>

#include <boost/fusion/container/map.hpp>
#include <boost/fusion/sequence/intrinsic/at_key.hpp>
#include <boost/fusion/sequence/intrinsic/value_at_key.hpp>

#include <boost/network/uri.hpp>
#include <boost/network/protocol/http/header.hpp>
#include <boost/network/traits/vector.hpp>

namespace boost { namespace network { namespace http {

    /** request.hpp
      *
      * This file implements the basic request object required
      * by the HTTP client implementation. The basic_request
      * object encapsulates a URI which is parsed at runtime.
      */

    template <class Tag>
    class basic_request : public basic_message<Tag>
    {

        boost::network::uri::http::uri uri_;

    public:
        typedef Tag tag;
        typedef typename string<Tag>::type string_type;

        explicit basic_request(string_type const & uri_)
        : uri_(uri_) 
        { }

        void uri(string_type const & new_uri) {
            uri_ = new_uri;
        }

        basic_request() 
        : basic_message<Tag>()
        { }

        basic_request(basic_request const & other) 
        : basic_message<Tag>(other), uri_(other.uri_)
        { }

        basic_request & operator=(basic_request rhs) {
            rhs.swap(*this);
            return *this;
        }

        void swap(basic_request & other) {
            using boost::network::uri::swap;
            basic_message<Tag> & base_ref(other);
            basic_message<Tag> & this_ref(*this);
            base_ref.swap(this_ref);
            swap(other.uri_, this->uri_);
        }

        string_type const host() const {
            return uri_.host();
        }

        unsigned int port() const {
            return uri_.port();
        }

        string_type const path() const {
            return uri_.path();
        }

        string_type const query() const {
            return uri_.query();
        }

        string_type const anchor() const {
            return uri_.fragment();
        }

        string_type const protocol() const {
            return uri_.protocol();
        }

    };

    /** This is the implementation of a POD request type
     *  that is specificially used by the HTTP server
     *  implementation. This fully specializes the
     *  basic_request template above to be
     *  primarily and be solely a POD for performance
     *  reasons.
     */
    template <>
    struct basic_request<tags::http_server> {
        typedef tags::http_server tag;
        typedef string<tags::http_server>::type string_type;
        typedef vector<tags::http_server>::apply<request_header>::type vector_type;
        string_type method;
        string_type uri;
        boost::uint8_t http_version_major;
        boost::uint8_t http_version_minor;
        vector_type headers;
        string_type body;

        void swap(basic_request & r) {
            using std::swap;
            swap(method, r.method);
            swap(uri, r.uri);
            swap(http_version_major, r.http_version_major);
            swap(http_version_minor, r.http_version_minor);
            swap(headers, r.headers);
            swap(body, r.body);
        }
    };

    template <class Tag>
    inline void swap(basic_request<Tag> & lhs, basic_request<Tag> & rhs) {
        lhs.swap(rhs);
    }

} // namespace http

} // namespace network

} // namespace boost

#endif // __NETWORK_PROTOCOL_HTTP_REQUEST_IMPL_20070908_1_HPP__

