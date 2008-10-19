
//          Copyright Dean Michael Berris 2007.
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

#include <boost/spirit.hpp>
#include <boost/spirit/phoenix.hpp>

namespace boost { namespace network { namespace http {

    /** request.hpp
      *
      * This file implements the basic request object required
      * by the HTTP client implementation. The basic_request
      * object encapsulates a URI which is parsed at runtime.
      * The data is broken up according to the URI specifications
      * RFC 3986 -- http://www.ietf.org/rfc/rfc3986.txt -- and
      * represented as a Fusion Map.
      */

    template <class tag>
    class basic_request :
        public basic_message<tag>
    {
        struct tags {
            struct protocol { };
            struct host { };
            struct port { };
            struct path { };
            struct query { };
            struct anchor { };
        };

        typedef fusion::map<
            fusion::pair<typename tags::protocol,typename string_traits<tag>::type>,
            fusion::pair<typename tags::host,typename string_traits<tag>::type>,
            fusion::pair<typename tags::port,unsigned int>,
            fusion::pair<typename tags::path,typename string_traits<tag>::type>,
            fusion::pair<typename tags::query,typename string_traits<tag>::type>,
            fusion::pair<typename tags::anchor,typename string_traits<tag>::type>
        > uri_parts_type;

        mutable uri_parts_type uri_parts;

    public:
        explicit basic_request(typename string_traits<tag>::type const & uri_) {
        	uri(uri_);
        }

        void uri(typename string_traits<tag>::type const & uri_) {
            using namespace boost::spirit;
            using namespace phoenix;

            fusion::at_key<typename tags::port>(uri_parts) = 80u;

            parse(
                uri_.begin(), uri_.end(),
                // the parser
                !(
                		str_p("http")[
                		              var(fusion::at_key<typename tags::protocol>(uri_parts))
                		              = construct_<typename string_traits<tag>::type>(arg1, arg2)
                		             ]
                		>> str_p("://")
                )
                >> (+(alnum_p | '.' | '-' | '_'))[
                    var(fusion::at_key<typename tags::host>(uri_parts))
                        = construct_<typename string_traits<tag>::type>(arg1, arg2)
                ]
                >> !(
                    ch_p(':')
                    >> uint_p[
                        var(fusion::at_key<typename tags::port>(uri_parts))
                            = arg1
                    ]
                    >> !ch_p('/')
                )
                >> (+(alnum_p | '/' | '%' | '_' | '-' | '.'))[
                    var(fusion::at_key<typename tags::path>(uri_parts))
                        = construct_<typename string_traits<tag>::type>(arg1, arg2)
                ]
                >> !(ch_p('?')
                    >> (+(alnum_p | '&' | '=' | '%' | '_' ))[
                        var(fusion::at_key<typename tags::query>(uri_parts))
                            = construct_<typename string_traits<tag>::type>(arg1, arg2)
                    ]
                )
                >> !(ch_p('#')
                    >> (+(alnum_p | '_' | '%' | '-'))[
                        var(fusion::at_key<typename tags::anchor>(uri_parts))
                            = construct_<typename string_traits<tag>::type>(arg1, arg2)
                    ]
                )
                >> end_p
                ,
                nothing_p // no skip parser
                );

            // check if path is empty, set it to '/'
            if (fusion::at_key<typename tags::path>(uri_parts).empty()) {
                fusion::at_key<typename tags::path>(uri_parts) = "/";
            }
        }

        basic_request() :
            basic_message<tag>(), uri_parts()
        { }

        basic_request(basic_request const & other) :
        basic_message<tag>(other), uri_parts(other.uri_parts)
        { }

        basic_request & operator=(basic_request rhs) {
            rhs.swap(*this);
            return *this;
        }

        void swap(basic_request & other) {
            basic_message<tag> & base_ref(other);
            basic_message<tag> & this_ref(*this);
            base_ref.swap(this_ref);
            uri_parts_type & other_uri_parts(other.uri_parts);
            uri_parts_type & this_uri_parts(this->uri_parts);
            std::swap(other_uri_parts, this_uri_parts);
        }

        typename string_traits<tag>::type const host() const {
            return fusion::at_key<typename tags::host>(uri_parts);
        }

        unsigned int port() const {
            return fusion::at_key<typename tags::port>(uri_parts);
        }

        typename string_traits<tag>::type const path() const {
            return fusion::at_key<typename tags::path>(uri_parts);
        }

        typename string_traits<tag>::type const query() const {
            return fusion::at_key<typename tags::query>(uri_parts);
        }

        typename string_traits<tag>::type const anchor() const {
            return fusion::at_key<typename tags::anchor>(uri_parts);
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
