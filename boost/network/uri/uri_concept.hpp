#ifndef BOOST_NETWORK_URL_URL_CONCEPT_HPP_
#define BOOST_NETWORK_URL_URL_CONCEPT_HPP_

// Copyright 2009 Dean Michael Berris.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/concept_check.hpp>

namespace boost { namespace network { namespace uri {

    template <class U>
        struct URI : DefaultConstructible<U>, EqualityComparable<U> {
            typedef typename U::string_type string_type;

            BOOST_CONCEPT_USAGE(URI)
            {
                U uri_(uri); // copy constructable
                U temp;
                swap(temp, uri_); // swappable
                string_type protocol_ = protocol(uri); // support protocol function
                string_type rest_ = rest(uri);
                bool valid_ = valid(uri);
                valid_ = false;
            }

            private:
            U uri;
        };

} // namespace uri

} // namespace network

} // namespace boost

#endif

