#ifndef BOOST_NETWORK_PROTOCOL_HTTP_REQUEST_CONCEPT_HPP_20100603
#define BOOST_NETWORK_PROTOCOL_HTTP_REQUEST_CONCEPT_HPP_20100603

// Copyright 2010 (c) Dean Michael Berris.
// Copyright 2010 (c) Sinefunc, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/concept_check.hpp>
#include <boost/network/message/message_concept.hpp>
#include <boost/cstdint.hpp>

namespace boost { namespace network { namespace http {

    template <class R>
    struct Request
        : boost::network::Message<R>
    {
        typedef typename R::string_type string_type;
        typedef typename R::port_type port_type;
        
        BOOST_CONCEPT_USAGE(Request) {
            string_type tmp;
            R request_(tmp);
            swap(request, request_); // swappable via ADL

            string_type host_ = host(request);
            port_type port_ = port(request);
            string_type path_ = path(request);
            string_type query_ = query(request);
            string_type anchor_ = anchor(request);
            string_type protocol_ = protocol(request);

            request << uri(string_type());

            boost::network::http::uri(request, string_type());

            (void)host_;
            (void)port_;
            (void)path_;
            (void)query_;
            (void)anchor_;
            (void)protocol_;
        }

    private:
        R request;
    };

} // namespace http

} // namespace network

} // namespace boost

#endif // BOOST_NETWORK_PROTOCOL_HTTP_REQUEST_CONCEPT_HPP_20100603
