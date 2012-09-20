// Copyright 2010 (c) Dean Michael Berris.
// Copyright 2010 (c) Sinefunc, Inc.
// Copyright 2012 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef NETWORK_PROTOCOL_HTTP_RESPONSE_CONCEPT_HPP_20100603
#define NETWORK_PROTOCOL_HTTP_RESPONSE_CONCEPT_HPP_20100603

#include <boost/concept_check.hpp>
#include <network/message/message_concept.hpp>
#include <boost/cstdint.hpp>

namespace network { namespace http {

    template <class R>
    struct Response
        : boost::network::Message<R>
    {
        typedef typename R::string_type string_type;
        
        BOOST_CONCEPT_USAGE(Response) {
            R response_;
            swap(response, response_); // swappable via ADL

            typedef std::string version_type;
            typedef std::string status_type;
            typedef std::string status_message_type;

            response << version(version_type()) // version directive
                << status(status_type()) // status directive
                << status_message(status_message_type()) // status_message directive
                ;

            version(response, version_type());
            status(response, status_type());
            status_message(response, status_message_type());

            string_type version_ = version(response);
            boost::uint16_t status_ = status(response);
            string_type status_message_ = status_message(response);

            (void)version_;
            (void)status_;
            (void)status_message_;
        }

    private:
        R response;
    };

} // namespace http
} // namespace network

#endif // NETWORK_PROTOCOL_HTTP_RESPONSE_CONCEPT_HPP_20100603
