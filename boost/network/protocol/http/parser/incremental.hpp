#ifndef BOOST_NETWORK_PROTOCOL_HTTP_PARSER_INCREMENTAL_HPP_20100909
#define BOOST_NETWORK_PROTOCOL_HTTP_PARSER_INCREMENTAL_HPP_20100909

//          Copyright Dean Michael Berris 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/tags.hpp>
#include <boost/range.hpp>
#include <boost/fusion/tuple.hpp>
#include <boost/network/traits/string.hpp>
#include <boost/logic/tribool.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <utility>

namespace boost { namespace network { namespace http {
    
    template <class Tag>
    struct response_parser {

        enum state_t {
            http_response_begin,
            http_version_h,
            http_version_t1,
            http_version_t2,
            http_version_p,
            http_version_slash,
            http_version_major,
            http_version_dot,
            http_version_minor,
            http_version_done,
            http_status_digit,
            http_status_done,
            http_status_message_char,
            http_status_message_cr,
            http_status_message_done
        };

        typedef typename string<Tag>::type::const_iterator iterator_type;
        typedef iterator_range<iterator_type>  range_type;

        explicit response_parser (state_t state=http_response_begin)
            : state_(state) {}

        response_parser (response_parser const & other) 
            : state_(other.state_) {}

        ~response_parser () {}
    
        void swap(response_parser & other) {
            std::swap(other.state_, this->state_);
        }

        response_parser & operator=(response_parser rhs) {
            rhs.swap(*this);
            return *this;
        }

        template <class Range>
        fusion::tuple<logic::tribool,range_type> parse_until(state_t stop_state, Range & range_) {
            logic::tribool parsed_ok(logic::indeterminate);
            iterator_type start = boost::begin(range_),
                current = start,
                end = boost::end(range_);
            range_type local_range = boost::make_iterator_range(start, end);
            while (!boost::empty(local_range) && indeterminate(parsed_ok)) {
                current = boost::begin(local_range);
                if (state_ == stop_state) {
                    parsed_ok = true;
                } else {
                    switch(state_) {
                    case http_response_begin:
                        if (*current == ' ' || *current == '\r' || *current == '\n') {
                            // skip valid leading whitespace
                            ++start;
                            ++current;
                        } else if (*current == 'H') {
                            state_ = http_version_h;
                            start = current;
                            ++current;
                        } else {
                            parsed_ok = false;
                        }
                        break;
                    case http_version_h:
                        if (*current == 'T') {
                            state_ = http_version_t1;
                            ++current;
                        } else {
                            parsed_ok = false;
                        }
                        break;
                    case http_version_t1:
                        if (*current == 'T') {
                            state_ = http_version_t2;
                            ++current;
                        } else {
                            parsed_ok = false;
                        }
                        break;
                    case http_version_t2:
                        if (*current == 'P') {
                            state_ = http_version_p;
                            ++current;
                        } else {
                            parsed_ok = false;
                        }
                        break;
                    case http_version_p:
                        if (*current == '/') {
                            state_ = http_version_slash;
                            ++current;
                        } else {
                            parsed_ok = false;
                        }
                        break;
                    case http_version_slash:
                        // FIXME find a better way to use is_digit
                        if (algorithm::is_digit()(*current)) {
                            state_ = http_version_major;
                            ++current;
                        } else {
                            parsed_ok = false;
                        }
                        break;
                    case http_version_major:
                        if (*current == '.') {
                            state_ = http_version_dot;
                            ++current;
                        } else {
                            parsed_ok = false;
                        }
                        break;
                    case http_version_dot:
                        // FIXME find a better way to use is_digit
                        if (algorithm::is_digit()(*current)) {
                            state_ = http_version_minor;
                            ++current;
                        } else {
                            parsed_ok = false;
                        }
                        break;
                    case http_version_minor:
                        if (*current == ' ') {
                            state_ = http_version_done;
                            ++current;
                        } else {
                            parsed_ok = false;
                        }
                        break;
                    case http_version_done:
                        // FIXME find a better way to use is_digit
                        if (algorithm::is_digit()(*current)) {
                            state_ = http_status_digit;
                            ++current;
                        } else {
                            parsed_ok = false;
                        }
                        break;
                    case http_status_digit:
                        // FIXME find a better way to use is_digit
                        if (algorithm::is_digit()(*current)) {
                            ++current;
                        } else if (*current == ' ') {
                            state_ = http_status_done;
                            ++current;
                        } else {
                            parsed_ok = false;
                        }
                        break;
                    case http_status_done:
                        // FIXME find a better way to use is_alnum, is_space
                        if (algorithm::is_alnum()(*current)) {
                            state_ = http_status_message_char;
                            ++current;
                        } else {
                            parsed_ok = false;
                        }
                        break;
                    case http_status_message_char:
                        // FIXME find a better way to use is_alnum, is_space
                        if (algorithm::is_alnum()(*current) || algorithm::is_punct()(*current) || (*current == ' ')) {
                            ++current;
                        } else if (*current == '\r') {
                            state_ = http_status_message_cr;
                            ++current;
                        } else {
                            parsed_ok = false;
                        }
                        break;
                    case http_status_message_cr:
                        if (*current == '\n') {
                            state_ = http_status_message_done;
                            ++current;
                        } else {
                            parsed_ok = false;
                        }
                        break;
                    default:
                        parsed_ok = false;
                    }
                }

                local_range = boost::make_iterator_range(current, end);
            }
            if (state_ == stop_state) parsed_ok = true;
            else parsed_ok = false;
            return fusion::make_tuple(parsed_ok,boost::make_iterator_range(start, current));
        }

        state_t state() {
            return state_;
        }

        void reset(state_t new_state = http_response_begin) {
            state_ = new_state;
        }

    private:
        state_t state_;

    };
    

} /* http */

} /* network */
    
} /* boost */

#endif
