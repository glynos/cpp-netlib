
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

namespace boost { namespace network { namespace http {

    /** request.hpp
      *
      * This file implements the basic request object required
      * by the HTTP client implementation. The basic_request
      * object encapsulates a URI which is parsed at runtime.
      */

    template <class Tag>
    class basic_request :
        public basic_message<Tag>
    {

        boost::network::uri::http::uri uri_;

    public:
        explicit basic_request(typename string_traits<Tag>::type const & uri_)
        : uri_(uri_) 
        { }

        void uri(typename string_traits<Tag>::type const & new_uri) {
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

        typename string_traits<Tag>::type const host() const {
            return uri_.host();
        }

        unsigned int port() const {
            return uri_.port();
        }

        typename string_traits<Tag>::type const path() const {
            return uri_.path();
        }

        typename string_traits<Tag>::type const query() const {
            return uri_.query();
        }

        typename string_traits<Tag>::type const anchor() const {
            return uri_.fragment();
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
