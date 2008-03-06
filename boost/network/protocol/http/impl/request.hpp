
//          Copyright Dean Michael Berris 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef __NETWORK_PROTOCOL_HTTP_REQUEST_IMPL_20070908_1_HPP__
#define __NETWORK_PROTOCOL_HTTP_REQUEST_IMPL_20070908_1_HPP__

#include <boost/fusion/sequence/container/map.hpp>
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
    class basic_request {
        struct tags {
            struct protocol { };
            struct host { };
            struct port { };
            struct path { };
            struct query { };
            struct anchor { };
        };

        typedef fusion::map<
            fusion::pair<typename tags::protocol,typename tag::str_type>,
            fusion::pair<typename tags::host,typename tag::str_type>,
            fusion::pair<typename tags::port,unsigned int>,
            fusion::pair<typename tags::path,typename tag::str_type>,
            fusion::pair<typename tags::query,typename tag::str_type>,
            fusion::pair<typename tags::anchor,typename tag::str_type>
        > uri_parts_type;
        
        mutable uri_parts_type uri_parts;

    public:
        explicit basic_request(typename tag::str_type const & uri) {
            using namespace boost::spirit;
            using namespace phoenix;

            fusion::at_key<typename tags::port>(uri_parts) = 80u;

            parse(
                uri.begin(), uri.end(),
                // the parser
                str_p("http")[
                    var(fusion::at_key<typename tags::protocol>(uri_parts))
                        = construct_<typename tag::str_type>(arg1, arg2)
                ]
                >> str_p("://")
                >> (+(alnum_p | '.' | '-' | '_'))[
                    var(fusion::at_key<typename tags::host>(uri_parts))
                        = construct_<typename tag::str_type>(arg1, arg2)
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
                        = construct_<typename tag::str_type>(arg1, arg2)
                ]
                >> !(ch_p('?')
                    >> (+(alnum_p | '&' | '=' | '%' | '_' ))[
                        var(fusion::at_key<typename tags::query>(uri_parts))
                            = construct_<typename tag::str_type>(arg1, arg2)
                    ]
                )
                >> !(ch_p('#')
                    >> (+(alnum_p | '_' | '%' | '-'))[
                        var(fusion::at_key<typename tags::anchor>(uri_parts))
                            = construct_<typename tag::str_type>(arg1, arg2)
                    ]
                )
                >> end_p
                ,
                nothing_p // no skip parser
                );
        };

        // conversion from a message object into a basic_request
        basic_request(basic_message<tag> const & message_) {
            //TODO: contruct from a network message object
        };

        basic_request(basic_request const & other) : 
        uri_parts(other.uri_parts)
        { };

        typename tag::str_type const host() const {
            return fusion::at_key<typename tags::host>(uri_parts);
        };

        unsigned int port() const {
            return fusion::at_key<typename tags::port>(uri_parts);
        };

        typename tag::str_type const path() const {
            return fusion::at_key<typename tags::path>(uri_parts);
        };

        typename tag::str_type const query() const {
            return fusion::at_key<typename tags::query>(uri_parts);
        };

        typename tag::str_type const anchor() const {
            return fusion::at_key<typename tags::anchor>(uri_parts);
        };
    };

}; // namespace http

}; // namespace network

}; // namespace boost

#endif // __NETWORK_PROTOCOL_HTTP_REQUEST_IMPL_20070908_1_HPP__
