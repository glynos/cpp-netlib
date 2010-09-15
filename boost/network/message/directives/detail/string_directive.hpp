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

namespace boost { namespace network { namespace detail {

    /** This directive template technically implements a decorator pattern
     *  which takes a concrete implementation and uses that as a base. This
     *  particular directive template requires the Base type to just include
     *  a nested template named `string_visitor` that takes a single template
     *  class parameter which is supposed to be a Message class/struct.
     *
     *  A typical directive implementation of the visitor will take a reference
     *  to a message as the constructor parameter and then performs operations
     *  on that message. An example implementation of a directive base is given
     *  below:
     *
     *      struct foo_directive_base {
     *          template <class Message>
     *          struct string_visitor : boost::static_visitor<> {
     *              Message const & message_;
     *              string_visitor(Message const & message)
     *                  : message_(message) {}
     *
     *              void operator()(
     *                  typename detail::string_value<typename Message::tag>::type
     *                  const & foo)
     *              const {
     *                  // do something to message_ here.
     *              }
     *
     *              template <class T> void operator()(T const &) const {
     *                  // fail at runtime?
     *              }
     *          };
     *      };
     *
     *  TODO -- distill this pattern into a preprocessor macro.
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
    
} /* detail */
    
} /* network */
    
} /* boost */

#endif /* BOOST_NETWORK_MESSAGE_DIRECTIVES_DETAIL_STRING_DIRECTIVE_HPP_20100915 */
