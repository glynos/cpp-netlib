#ifndef BOOST_NETWORK_MESSAGE_DIRECTIVES_DETAIL_STRING_DIRECTIVE_HPP_20100915
#define BOOST_NETWORK_MESSAGE_DIRECTIVES_DETAIL_STRING_DIRECTIVE_HPP_20100915

//          Copyright Dean Michael Berris 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/traits/string.hpp>
#include <boost/variant/variant.hpp>
#include <boost/variant/apply_visitor.hpp>
#include <boost/variant/static_visitor.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/or.hpp>
#include <boost/network/message/directives/detail/string_value.hpp>

namespace boost { namespace network { namespace detail {

    /** This directive template technically implements a decorator pattern
     *  which takes a concrete implementation and uses that as a base. This
     *  particular directive template requires the Base type to just include
     *  a nested template named `string_visitor` that takes a single template
     *  class parameter which is supposed to be a Message class/struct.
     *
     *  A typical directive implementation of the visitor will take a reference
     *  to a message as the constructor parameter and then performs operations
     *  on that message. 
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
    template <class Base>
    struct string_directive : public Base {
        boost::variant<
            string<tags::default_string>::type,
            string<tags::default_wstring>::type,
            boost::shared_future<string<tags::default_string>::type>,
            boost::shared_future<string<tags::default_wstring>::type>
        > string_;

        explicit string_directive(string<tags::default_string>::type const & input)
            : string_(input) {}
        explicit string_directive(string<tags::default_wstring>::type const & input)
            : string_(input) {}
        explicit string_directive(boost::shared_future<string<tags::default_string>::type> const & input)
            : string_(input) {}
        explicit string_directive(boost::shared_future<string<tags::default_wstring>::type> const & input)
            : string_(input) {}

        string_directive(string_directive const & other)
            : string_(other.string_) {}

        template <class Tag, template <class> class Message>
        void operator()(Message<Tag> const & message) const {
            apply_visitor(typename Base::template string_visitor<Message<Tag> >(message), string_);
        }
    };
    
#define BOOST_NETWORK_STRING_DIRECTIVE(name, value, body, pod_body)         \
    struct name##_directive_base {                                          \
                                                                            \
        template <class Message>                                            \
        struct normal_visitor : boost::static_visitor<> {                   \
            Message const & message;                                        \
            explicit normal_visitor(Message const & message) :              \
                message(message) {}                                         \
            void operator()(                                                \
                typename boost::network::detail::string_value<              \
                    typename Message::tag                                   \
                >::type const & value                                       \
            ) const {                                                       \
                body;                                                       \
            }                                                               \
            template <class T> void operator()(T const &) const {           \
            }                                                               \
        };                                                                  \
                                                                            \
        template <class Message>                                            \
        struct pod_visitor : boost::static_visitor<> {                      \
            Message const & message;                                        \
            explicit pod_visitor(Message const & message) :                 \
                message(message) {}                                         \
            void operator()(                                                \
                typename boost::network::detail::string_value<              \
                    typename Message::tag                                   \
                >::type const & value                                       \
            ) {                                                       \
                pod_body;                                                   \
            }                                                               \
            template <class T> void operator()(T const &) const {           \
            }                                                               \
        };                                                                  \
                                                                            \
        template <class Message>                                            \
        struct string_visitor :                                             \
            mpl::if_<                                                       \
                is_base_of<                                                 \
                    tags::pod,                                              \
                    typename Message::tag                                   \
                >,                                                          \
                pod_visitor<Message>,                                       \
                normal_visitor<Message>                                     \
            >::type                                                         \
        {                                                                   \
            typedef typename mpl::if_<                                      \
                is_base_of<                                                 \
                    tags::pod,                                              \
                    typename Message::tag                                   \
                >,                                                          \
                pod_visitor<Message>,                                       \
                normal_visitor<Message>                                     \
            >::type base;                                                   \
            explicit string_visitor(Message const & message):               \
                base(message) {}                                            \
            string_visitor(string_visitor const & other):                   \
                base(other) {}                                              \
            using base::operator();                                         \
        };                                                                  \
    };                                                                      \
                                                                            \
    typedef boost::network::detail::string_directive<name##_directive_base> \
        name##_directive;                                                   \
    template <class T> inline name##_directive const                        \
    name (T const & input) {                                                \
        return name##_directive(input);                                     \
    }

} /* detail */
    
} /* network */
    
} /* boost */

#endif /* BOOST_NETWORK_MESSAGE_DIRECTIVES_DETAIL_STRING_DIRECTIVE_HPP_20100915 */
