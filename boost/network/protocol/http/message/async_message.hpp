#ifndef BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_ASYNC_MESSAGE_HPP_20100622
#define BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_ASYNC_MESSAGE_HPP_20100622

// Copyright 2010 (c) Dean Michael Berris
// Copyright 2010 (c) Sinefunc, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/thread/future.hpp>
#include <boost/cstdint.hpp>

namespace boost { namespace network { namespace http {

    template <class Tag>
    struct async_message {

        typedef typename string<Tag>::type string_type;
        typedef typename headers_container<Tag>::type headers_container_type;

        async_message()
            : status_message_(),
            status_(),
            version_(),
            source_(),
            destination_(),
            headers_(),
            body_()
        {}

        async_message(async_message const & other)
            : status_message_(other.status_message_),
            status_(other.status_),
            version_(other.version_),
            source_(other.source_),
            destination_(other.destination_),
            headers_(other.destination_),
            body_(other.body_)
        {}

        string_type const status_message() const {
            return status_message_.get();
        }

        void status_message(boost::shared_future<string_type> const & future) const {
            status_message_ = future;
        }

        string_type const version() const {
            return version_.get();
        }

        void version(boost::shared_future<string_type> const & future) const {
            version_ = future;
        }

        boost::uint16_t const status() const {
            return status_.get();
        }

        void status(boost::shared_future<string_type> const & future) const {
            status_ = future;
        }

        string_type const source() const {
            return source_.get();
        }

        void source(boost::shared_future<string_type> const & future) const {
            source_ = future;
        }

        string_type const destination() const {
            return destination_.get();
        }

        void destination(boost::shared_future<string_type> const & future) const {
            destination_ = future;
        }

        headers_container_type const headers() const {
            return headers_.get();
        }

        void headers(boost::shared_future<headers_container_type> const & future) const {
            headers_ = future;
        }

        string_type const body() const {
            return body_.get();
        }

        void body(boost::shared_future<string_type> const & future) const {
            body_ = future;
        }

        void swap(async_message & other) {
            std::swap(status_message_, other.status_message_);
            std::swap(status_, other.status_);
            std::swap(version_, other.version_);
            std::swap(source_, other.source_);
            std::swap(destination_, other.destination_);
            std::swap(headers_, other.headers_);
            std::swap(body_, other.body_);
        }

        async_message & operator=(async_message other) {
            other.swap(*this);
            return *this;
        }

    private:

        mutable boost::shared_future<string_type> status_message_,
            version_, source_, destination_, body_;
        mutable boost::shared_future<boost::uint16_t> status_;
        mutable boost::shared_future<headers_container_type> headers_;

    };

    template <class Tag>
    inline void swap(async_message<Tag> & lhs, async_message<Tag> & rhs) {
        lhs.swap(rhs);
    }

} // namespace http

} // namespace network

} // namespace boost

#include <boost/network/protocol/http/message/wrappers/source.hpp>
#include <boost/network/protocol/http/message/wrappers/body.hpp>

#endif // BOOST_NETWORK_PROTOCOL_HTTP_MESSAGE_ASYNC_MESSAGE_HPP_20100622
