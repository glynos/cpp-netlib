#ifndef BOOST_NETWORK_URL_BASIC_URL_
#define BOOST_NETWORK_URL_BASIC_URL_

// Copyright 2009 Dean Michael Berris.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/fusion/adapted/std_pair.hpp>
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

            string_type protocol() const {
                return parts_.scheme;
            }

            string_type rest() const {
                return parts_.scheme_specific_part;
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
            using uri_base<Tag>::string_type;
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
        protocol(basic_uri<Tag> const & uri) {
            return uri.protocol();
        }

    template <class Tag>
        inline 
        typename string<Tag>::type 
        rest(basic_uri<Tag> const & uri) {
            return uri.rest();
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

