#ifndef BOOST_NETWORK_MESSAGE_DIRECTIVES_DETAIL_STRING_DIRECTIVE_HPP_20100915
#define BOOST_NETWORK_MESSAGE_DIRECTIVES_DETAIL_STRING_DIRECTIVE_HPP_20100915

//          Copyright Dean Michael Berris 2010.
// Copyright 2012 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <boost/variant/variant.hpp>
#include <boost/variant/apply_visitor.hpp>
#include <boost/variant/static_visitor.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/or.hpp>

/**
 *
 *  To create your own string directive, you can use the preprocessor macro
 *  BOOST_NETWORK_STRING_DIRECTIVE which takes three parameters: the name of
 *  the directive, a name for the variable to use in the directive visitor,
 *  and the body to be implemented in the visitor. An example directive for
 *  setting the source of a message would look something like this given the
 *  BOOST_NETWORK_STRING_DIRECTIVE macro:
 *
 *      BOOST_NETWORK_STRING_DIRECTIVE(source, source_, 
 *          message.source(source_)
 *          , message.source=source_);
 *
 */
    
#ifndef BOOST_NETWORK_STRING_DIRECTIVE
#define BOOST_NETWORK_STRING_DIRECTIVE(name)                                \
    struct name##_directive {                                               \
        std::string const & value;                                          \
        explicit name##_directive(std::string const & value_)               \
        : value(value_) {}                                                  \
        name##_directive(name##_directive const & other)                    \
        : value(other.value) {}                                             \
        template <class Message>                                            \
        void operator()(Message & message) const {                          \
            message.set_##name(value);                                      \
        }                                                                   \
    };                                                                      \
                                                                            \
    inline name##_directive const                                           \
    name (std::string const & input) {                                      \
        return name##_directive(input);                                     \
    }
#endif /* BOOST_NETWORK_STRING_DIRECTIVE */

#endif /* BOOST_NETWORK_MESSAGE_DIRECTIVES_DETAIL_STRING_DIRECTIVE_HPP_20100915 */
