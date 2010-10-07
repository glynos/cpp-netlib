#ifndef BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_DIRECTIVES_VERSION_HPP_20100603
#define BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_DIRECTIVES_VERSION_HPP_20100603

// Copyright 2010 (c) Dean Michael Berris
// Copyright 2010 (c) Sinefunc, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/tags.hpp>
#include <boost/network/support/is_sync.hpp>
#include <boost/network/support/is_async.hpp>
#include <boost/variant/variant.hpp>
#include <boost/variant/static_visitor.hpp>
#include <boost/mpl/if.hpp>

namespace boost { namespace network { namespace http {

    template <class Tag>
    struct basic_response;

    namespace impl {

        template <class Tag>
        struct unsupported_tag;
        
        struct version_directive {

            template <class Tag>
            struct value :
                mpl::if_<
                    is_async<Tag>,
                    boost::shared_future<typename string<Tag>::type>,
                    typename mpl::if_<
                        is_sync<Tag>,
                        typename string<Tag>::type,
                        unsupported_tag<Tag>
                    >::type
                >
            {};

            template <class Tag>
            struct version_visitor : boost::static_visitor<> {
                basic_response<Tag> const & response;
                version_visitor(basic_response<Tag> const & response_)
                    : response(response_) {}
                version_visitor(version_visitor const & other)
                    : response(other.response) {}
                
                void operator()(typename value<Tag>::type const & version) const {
                    response.version(version);
                }

                template <class T>
                void operator()(T const & version) const {
                    //FIXME it should fail here
                }
            };
            
            boost::variant<
                string<tags::default_string>::type, 
                string<tags::default_wstring>::type, 
                boost::shared_future<string<tags::default_string>::type>,
                boost::shared_future<string<tags::default_wstring>::type>
            > version_;

            version_directive(string<tags::default_string>::type const & version)
                : version_(version) {}

            version_directive(string<tags::default_wstring>::type const & version)
                : version_(version) {}

            version_directive(boost::shared_future<string<tags::default_string>::type> const & version)
                : version_(version) {}

            version_directive(boost::shared_future<string<tags::default_wstring>::type> const & version)
                : version_(version) {}

            version_directive(version_directive const & other)
                : version_(other.version_) {}

            template <class Tag> basic_response<Tag> const & operator() (basic_response<Tag> const & response) const {
                apply_visitor(version_visitor<Tag>(response), version_);
                return response;
            }
    
        };

    } // namespace impl

    template <class Input>
    inline impl::version_directive const version(Input const & version_) {
        return impl::version_directive(version_);
    }

} // namespace http

} // namespace network

} // namespace boost


#endif // BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_DIRECTIVES_VERSION_HPP_20100603
