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
        
        BOOST_CONECEPT_USAGE(Request) {
            R request_(string_type());
            swap(request_, request_); // swappable via ADL

            string_type host_ = host(request);
            port_type port = port(request);
            string_type path = path(request);

            request << host(string_type())
                << port(port_type())
                << path(string_type())
                ;

            host(request, string_type());
            port(request, port_type());
            path(request, string_type());
        }

    private:
        R request;
    };

} // namespace http

} // namespace network

} // namespace boost

#endif // BOOST_NETWORK_PROTOCOL_HTTP_REQUEST_CONCEPT_HPP_20100603
