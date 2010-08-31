
//          Copyright Dean Michael Berris 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef __NETWORK_MESSAGE_DIRECTIVES_SOURCE_HPP__
#define __NETWORK_MESSAGE_DIRECTIVES_SOURCE_HPP__

#include <boost/network/traits/string.hpp>
#include <boost/network/support/is_async.hpp>
#include <boost/network/support/is_sync.hpp>
#include <boost/variant/variant.hpp>
#include <boost/variant/apply_visitor.hpp>
#include <boost/variant/static_visitor.hpp>
#include <boost/thread/future.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/or.hpp>

/** source.hpp
*
* Defines the types involved and the semantics of adding
* source information into message objects.
*
* WARNING: DO NOT INCLUDE THIS HEADER DIRECTLY. THIS REQUIRES
* TYPES TO BE DEFINED FROM EARLIER FILES THAT INCLUDE THIS
* HEADER.
*/
namespace boost { namespace network {

    namespace traits {

        template <class Tag>
        struct unsupported_tag;

        template <class Message>
        struct source :
            mpl::if_<
            is_async<typename Message::tag>,
            boost::shared_future<typename string<typename Message::tag>::type>,
            typename mpl::if_<
            mpl::or_<
            is_same<typename Message::tag, tags::default_string>,
            is_same<typename Message::tag, tags::default_wstring>,
            is_sync<typename Message::tag>
            >,
            typename string<typename Message::tag>::type,
            unsupported_tag<typename Message::tag>
            >::type
            >
        {};

    } // namespace traits

    namespace impl {

        struct source_directive {

            boost::variant<
                typename string<tags::default_string>::type,
                typename string<tags::default_wstring>::type,
                boost::shared_future<string<tags::default_string>::type>,
                boost::shared_future<string<tags::default_wstring>::type>
            > source_;

            explicit source_directive (string<tags::default_string>::type const & source)
                : source_(source)
            { };
            explicit source_directive (string<tags::default_wstring>::type const & source)
                : source_(source)
            { };
            explicit source_directive (boost::shared_future<typename string<tags::default_string>::type> const & source)
                : source_(source)
            { };
            explicit source_directive (boost::shared_future<typename string<tags::default_wstring>::type> const & source)
                : source_(source)
            { };

            source_directive(source_directive const & other)
                : source_(other.source_) {}

            template <class Tag>
            struct value :
                mpl::if_<
                    is_async<Tag>,
                    boost::shared_future<typename string<Tag>::type>,
                    typename mpl::if_<
                        mpl::or_<
                            is_sync<Tag>,
                            is_same<Tag, tags::default_string>,
                            is_same<Tag, tags::default_wstring>
                        >,
                        typename string<Tag>::type,
                        unsupported_tag<Tag>
                    >::type
                >
            {};

            template <class Message>
            struct source_visitor : boost::static_visitor<> {
                Message const & message_;
                source_visitor(Message const & message)
                    : message_(message) {}
                source_visitor(source_visitor const & other)
                    : message_(other.message_) {}
                void operator()(typename value<typename Message::tag>::type const & source) const {
                    message_.source(source);
                }
                template <class T> void operator()(T const &) const {
                    // FIXME -- fail here!
                }
            };

            template <class Tag, template <class> class Message>
            void operator() (Message<Tag> const & msg) const {
                apply_visitor(source_visitor<Message<Tag> >(msg), source_);
            }
            
        };

    } // namespace impl

    template <class Input>
    inline impl::source_directive const source(Input const & input) {
        return impl::source_directive(input);
    }

} // namespace network
} // namespace boost

#endif // __NETWORK_MESSAGE_DIRECTIVES_SOURCE_HPP__

