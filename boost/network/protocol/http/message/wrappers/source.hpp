#ifndef BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_WRAPPERS_SOURCE_HPP_20100622
#define BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_WRAPPERS_SOURCE_HPP_20100622

// Copyright 2010 (c) Dean Michael Berris
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/protocol/http/response_concept.hpp>
#include <boost/network/protocol/http/request_concept.hpp>
#include <boost/mpl/if.hpp>
#include <boost/concept/requires.hpp>

namespace boost { namespace network { namespace http {

    template <class Tag>
    struct basic_response;

    template <class Tag>
    struct basic_request;

#define BOOST_NETWORK_DEFINE_HTTP_WRAPPER(name, accessor)               \
    struct name##_pod_accessor {                                        \
    protected:                                                          \
        template <class Message>                                        \
        typename Message::string_type const &                           \
        get_value(Message const & message) const {                      \
            return message.accessor;                                    \
        }                                                               \
    };                                                                  \
                                                                        \
    struct name##_member_accessor {                                     \
    protected:                                                          \
        template <class Message>                                        \
        typename Message::string_type                                   \
        get_value(Message const & message) const {                      \
            return message.accessor();                                  \
        }                                                               \
    };                                                                  \
                                                                        \
    template <class Tag>                                                \
    struct name##_wrapper_impl :                                        \
        mpl::if_<                                                       \
            is_base_of<tags::pod, Tag>,                                 \
            name##_pod_accessor,                                        \
            name##_member_accessor                                      \
        >                                                               \
    {};                                                                 \
                                                                        \
    template <class Message>                                            \
    struct name##_wrapper :                                             \
    name##_wrapper_impl<typename Message::tag>::type {                  \
        typedef typename string<typename Message::tag>::type            \
            string_type;                                                \
        Message const & message_;                                       \
        name##_wrapper(Message const & message)                         \
            : message_(message) {}                                      \
        name##_wrapper(name##_wrapper const & other)                    \
            : message_(other.message_) {}                               \
        operator string_type const () {                                 \
            return this->get_value(message_);                                 \
        }                                                               \
    };                                                                  \
                                                                        \
    template <class Tag>                                                \
    inline BOOST_CONCEPT_REQUIRES(((Response<basic_response<Tag> >)),   \
        (name##_wrapper<basic_response<Tag> > const))                   \
    name (basic_response<Tag> const & message) {                        \
        return name##_wrapper<basic_response<Tag> >(message);           \
    }                                                                   \
                                                                        \
    template <class Tag>                                                \
    inline BOOST_CONCEPT_REQUIRES(((Request<basic_request<Tag> >)),     \
        (name##_wrapper<basic_request<Tag> > const))                    \
    name (basic_request<Tag> const & message) {                         \
        return name##_wrapper<basic_request<Tag> >(message);            \
    }

    BOOST_NETWORK_DEFINE_HTTP_WRAPPER(source, source);

} // namespace http

} // namespace network

} // namespace boost

#endif // BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_WRAPPERS_SOURCE_HPP_20100622
