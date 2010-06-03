//          Copyright Dean Michael Berris 2007.
//          Copyright Michael Dickey 2008.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_NETWORK_PROTOCOL_HTTP_RESPONSE_HPP
#define BOOST_NETWORK_PROTOCOL_HTTP_RESPONSE_HPP

#include <boost/network/protocol/http/message.hpp>
#include <boost/network/message.hpp>

#include <boost/cstdint.hpp>
#include <boost/network/protocol/http/response_concept.hpp>

namespace boost { namespace network { namespace http {

    template <class Tag>
    struct basic_response : public message_impl<Tag> {
    private:
        typedef message_impl<Tag> base_type;

        mutable string_type version_;
        mutable boost::uint16_t status_;
        mutable string_type status_message_;
    public:

        typedef Tag tag;
        typedef typename string<Tag>::type string_type;

        basic_response()
        : base_type(), version_(), status_(0u), status_message_()
        { };

        basic_response(basic_response const & other)
        : base_type(other), version_(other.version_), status_(other.status_), status_message_(other.status_message_)
        { };

        string_type & version() const {
            return version_;
        };

        boost::uint16_t & status() const {
            return status_;
        };

        string_type & status_message() const {
            return status_message_;
        };

        basic_response & operator=(basic_response rhs) {
            rhs.swap(*this);
            return *this;
        };

        void swap(basic_response & other) {
            message_impl<Tag> & base_ref(other);
            message_impl<Tag> & this_ref(*this);
            this_ref.swap(base_ref);
            std::swap(other.version_, version_);
            std::swap(other.status_, status_);
            std::swap(other.status_message_, status_message_);
        };
    };

    template <class Tag>
    inline void swap(basic_response<Tag> & lhs, basic_response<Tag> & rhs) {
        lhs.swap(rhs);
    }

    BOOST_CONCEPT_ASSERT((Response<basic_response<tags::http_default_8bit_udp_resolve> >));

} // namespace http

} // namespace network

} // namespace boost

#include <boost/network/protocol/http/impl/response.ipp>

namespace boost { namespace network { namespace http {

    template <class Tag, class Directive>
    basic_response<Tag> & operator<<(
        basic_response<Tag> & message,
        Directive const & directive
        )
    {
        directive(message);
        return message;
    }

} // namespace http

} // namespace network

} // namespace boost

#include <boost/network/protocol/http/message/directives/status_message.hpp>
#include <boost/network/protocol/http/message/directives/version.hpp>
#include <boost/network/protocol/http/message/directives/status.hpp>

#include <boost/network/protocol/http/message/wrappers/version.hpp>
#include <boost/network/protocol/http/message/wrappers/status.hpp>
#include <boost/network/protocol/http/message/wrappers/status_message.hpp>

#endif // BOOST_NETWORK_PROTOCOL_HTTP_RESPONSE_HPP
