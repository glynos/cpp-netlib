#ifndef BOOST_NETWORK_PROTOCOL_HTTP_SERVER_REQUEST_PARSER_HPP_20101005
#define BOOST_NETWORK_PROTOCOL_HTTP_SERVER_REQUEST_PARSER_HPP_20101005

// Copyright 2010 Dean Michael Berris. 
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <utility>
#include <boost/range/iterator_range.hpp>
#include <boost/logic/tribool.hpp>
#include <boost/fusion/tuple.hpp>

namespace boost { namespace network { namespace http {

    template <class Tag>
    struct request_parser {

        enum state_t {
            method_start
            , method_char
            , method_done
            , uri_char
            , uri_done
            , version_h
            , version_t1
            , version_t2
            , version_p
            , version_d1
            , version_dot
            , version_d2
            , version_done
            , header_name
            , header_colon
            , header_value
            , header_line_done
            , headers_done
        };

        template <class Range>
        fusion::tuple<logic::tribool, Range>
        parse_until(state_t state, Range const & range) {
            logic::tribool parsed_ok;
            typedef typename range_iterator<Range>::type iterator;
            iterator start = boost::begin(range)
                    , end  = boost::end(range)
                    , current_iterator = start;

            while (!boost::empty(range) || state != internal_state) {
                switch(internal_state) {
                    default:
                        parsed_ok = false;
                        return fusion::make_tuple(
                            parsed_ok, 
                            boost::make_iterator_range(
                                start, current_iterator
                                )
                            );
                };
            }
        }

    private:
        state_t internal_state;

    };
    
} /* http */
    
} /* network */
    
} /* boost */

#endif /* BOOST_NETWORK_PROTOCOL_HTTP_SERVER_REQUEST_PARSER_HPP_20101005 */
