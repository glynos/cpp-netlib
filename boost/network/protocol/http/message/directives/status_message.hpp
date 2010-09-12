#ifndef BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_DIRECTIVES_STATUS_MESSAGE_HPP_20100603
#define BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_DIRECTIVES_STATUS_MESSAGE_HPP_20100603

// Copyright 2010 (c) Dean Michael Berris
// Copyright 2010 (c) Sinefunc, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/tags.hpp>
#include <boost/network/support/is_async.hpp>
#include <boost/network/support/is_sync.hpp>
#include <boost/variant/variant.hpp>
#include <boost/variant/static_visitor.hpp>
#include <boost/variant/apply_visitor.hpp>

namespace boost { namespace network { namespace http {

    template <class Tag>
    struct basic_response;

    namespace impl {

        struct status_message_directive {
            
            boost::variant<
                string<tags::default_string>::type,
                string<tags::default_wstring>::type,
                boost::shared_future<string<tags::default_string>::type>,
                boost::shared_future<string<tags::default_wstring>::type>
            > status_message_;

            status_message_directive(string<tags::default_string>::type const & status_message)
                : status_message_(status_message) {}

            status_message_directive(string<tags::default_wstring>::type const & status_message)
                : status_message_(status_message) {}

            status_message_directive(boost::shared_future<string<tags::default_string>::type> const & status_message)
                : status_message_(status_message) {}

            status_message_directive(boost::shared_future<string<tags::default_wstring>::type> const & status_message)
                : status_message_(status_message) {}

            status_message_directive(status_message_directive const & other)
                : status_message_(other.status_message_) {}
            
            template <class Tag>
            struct unsupported_tag;

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
            struct status_message_visitor : boost::static_visitor<> {
                basic_response<Tag> const & response;
                status_message_visitor(basic_response<Tag> const & response)
                    : response(response) {}
                void operator() (typename value<Tag>::type const & status_message) const {
                    response.status_message(status_message);
                }
                template <class T>
                void operator() (T const &) const {
                    // FIXME fail here!
                }
            };

            template <class Tag> 
            basic_response<Tag> const & operator() (basic_response<Tag> const & response) const {
                apply_visitor(status_message_visitor<Tag>(response), status_message_);
                return response;
            }

        };

    } // namespace impl

    template <class T>
    inline impl::status_message_directive const status_message(T const & status_message_) {
        return impl::status_message_directive(status_message_);
    }

} // namespace http

} // namespace network 

} // namespace boost

#endif // BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_DIRECTIVES_STATUS_MESSAGE_HPP_20100603
