#ifndef BOOST_NETWORK_URL_BASIC_URL_
#define BOOST_NETWORK_URL_BASIC_URL_

// Copyright 2009 Dean Michael Berris, Jeroen Habraken.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/range.hpp>

#include <boost/network/tags.hpp>
#include <boost/network/uri/basic_uri_fwd.hpp>
#include <boost/network/uri/detail/parse_uri.hpp>
#include <boost/network/uri/uri_concept.hpp>

namespace boost { namespace network { namespace uri {

    template <class Tag>
        struct uri_base {
            typedef typename string<Tag>::type string_type;

            uri_base(string_type const & uri = string_type())            
                : 
                    raw_(uri), 
                    parts_(), 
                    valid_(parse_uri(raw_, parts_)) 
            { }

            uri_base(const uri_base & other)
                : 
                    raw_(other.raw_), 
                    parts_(other.parts_), 
                    valid_(other.valid_) 
            { }

            uri_base & operator=(uri_base other) {
                other.swap(*this);
                return *this; 
            }

            uri_base & operator=(string_type const & uri) {
                return *this = uri_base(uri);
            }

            void swap(uri_base & other) {
                using std::swap;

                swap(other.raw_, raw_);
                swap(other.parts_, parts_);
                swap(other.valid_, valid_);
            }

            string_type scheme() const {
                return parts_.scheme;
            }

            string_type user_info() const {
                return parts_.user_info ? *parts_.user_info : string_type();
            }

            string_type host() const {
                return parts_.host ? *parts_.host : string_type();
            }

            uint16_t port() const {
                return parts_.port ? *parts_.port : 0;
            }

            string_type path() const {
                return parts_.path;
            }

            string_type query() const {
                return parts_.query ? *parts_.query : string_type();
            }

            string_type fragment() const {
                return parts_.fragment ? *parts_.fragment : string_type();
            }

            bool valid() const {
                return valid_;
            }

            bool operator==(uri_base const & other) const {
                return (raw_ == other.raw_) && (parts_ == other.parts_) && (valid_ == other.valid_);
            }

            bool operator!=(uri_base const & other) const {
                return !(*this == other);
            }

            protected:

            string_type raw_;
            detail::uri_parts<Tag> parts_;
            bool valid_;
        };

    template <class Tag> 
        struct basic_uri : uri_base<Tag> {
            using uri_base<Tag>::operator=;
            using typename uri_base<Tag>::string_type;
            using uri_base<Tag>::operator==;
            using uri_base<Tag>::operator!=;
            basic_uri() : uri_base<Tag>() {}
            basic_uri(typename uri_base<Tag>::string_type const & uri) : uri_base<Tag>(uri) {}
        };

    template <class Tag>
        inline 
        void 
        swap(basic_uri<Tag> & left, basic_uri<Tag> & right) {
            right.swap(left);
        }

    template <class Tag>
        inline 
        typename string<Tag>::type 
        scheme(basic_uri<Tag> const & uri) {
            return uri.scheme();
        }

    template <class Tag>
        inline
        typename string<Tag>::type
        user_info(basic_uri<Tag> const & uri) {
            return uri.user_info();
        }

    template <class Tag>
        inline
        typename string<Tag>::type
        host(basic_uri<Tag> const & uri) {
            return uri.host();
        }

    template <class Tag>
        inline
        uint16_t
        port(basic_uri<Tag> const & uri) {
            return uri.port();
        }

    template <class Tag>
        inline
        typename string<Tag>::type
        path(basic_uri<Tag> const & uri) {
            return uri.path();
        }

    template <class Tag>
        inline
        typename string<Tag>::type
        query(basic_uri<Tag> const & uri) {
            return uri.query();
        }

    template <class Tag>
        inline
        typename string<Tag>::type
        fragment(basic_uri<Tag> const & uri) {
            return uri.fragment();
        }

    template <class Tag>
        inline 
        bool 
        valid(basic_uri<Tag> const & uri) {
            return uri.valid();
        }

    // Check that the URI concept is met by the basic_uri type.
BOOST_CONCEPT_ASSERT((URI<basic_uri<boost::network::tags::default_string> >));
BOOST_CONCEPT_ASSERT((URI<basic_uri<boost::network::tags::default_wstring> >));

} // namespace uri

} // namespace network

} // namespace boost

#endif

