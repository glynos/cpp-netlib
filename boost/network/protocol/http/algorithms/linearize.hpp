#ifndef BOOST_NETWORK_PROTOCOL_HTTP_ALGORITHMS_LINEARIZE_HPP_20101028
#define BOOST_NETWORK_PROTOCOL_HTTP_ALGORITHMS_LINEARIZE_HPP_20101028

// Copyright 2010 Dean Michael Berris.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/protocol/http/message/header/name.hpp>
#include <boost/network/protocol/http/message/header/value.hpp>
#include <boost/network/protocol/http/message/header_concept.hpp>
#include <boost/network/constants.hpp>
#include <boost/concept_check.hpp>

namespace boost { namespace network { namespace http {

    template <class Tag>
    struct linearize {
        typedef typename string<Tag>::type string_type;

        template <class Arguments>
        struct result;

        template <class This, class Arg>
        struct result<This(Arg)> {
            typedef string_type type;
        };

        template <class ValueType>
        BOOST_CONCEPT_REQUIRES(
            ((Header<ValueType>)),
            (string_type)
        ) operator()(ValueType & header) {
            typedef typename ostringstream<Tag>::type output_stream;
            typedef constants<Tag> consts;
            output_stream header_line;
            header_line << name(header) 
                << consts::colon() << consts::space() 
                << value(header) << consts::crlf();
            return header_line.str();
        }
    };
    
} /* linearize */
    
} /* net */
    
} /* boost */

#endif /* BOOST_NETWORK_PROTOCOL_HTTP_ALGORITHMS_LINEARIZE_HPP_20101028 */
