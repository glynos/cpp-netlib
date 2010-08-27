
//          Copyright Dean Michael Berris 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef __NETWORK_MESSAGE_DIRECTIVES_BODY_HPP__
#define __NETWORK_MESSAGE_DIRECTIVES_BODY_HPP__

#include <boost/network/traits/string.hpp>
#include <boost/network/support/is_async.hpp>
#include <boost/network/support/is_sync.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/variant/variant.hpp>
#include <boost/variant/apply_visitor.hpp>
#include <boost/variant/static_visitor.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/or.hpp>

namespace boost { namespace network {

    namespace traits {
        template <class Tag>
        struct unsupported_tag;

        template <class Message>
        struct body :
            mpl::if_<
                is_async<typename Message::tag>,
                boost::shared_future<typename string<typename Message::tag>::type>,
                typename mpl::if_<
                    mpl::or_<
                        is_sync<typename Message::tag>,
                        is_same<typename Message::tag, tags::default_string>,
                        is_same<typename Message::tag, tags::default_wstring>
                    >,
                    typename string<typename Message::tag>::type,
                    unsupported_tag<typename Message::tag>
                >::type
            >
        {};
    } // namespace traits

    namespace impl {

        struct body_directive {
            boost::variant<
                string<tags::default_string>::type,
                string<tags::default_wstring>::type,
                boost::shared_future<string<tags::default_string>::type>,
                boost::shared_future<string<tags::default_wstring>::type>
            > body_;

            body_directive(string<tags::default_string>::type const & body)
                : body_(body) {}
            body_directive(string<tags::default_wstring>::type const & body)
                : body_(body) {}
            body_directive(boost::shared_future<string<tags::default_string>::type> const & body)
                : body_(body) {}
            body_directive(boost::shared_future<string<tags::default_wstring>::type> const & body)
                : body_(body) {}

            body_directive(body_directive const & other)
                : body_(other.body_) {}

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
            struct body_visitor : boost::static_visitor<> {
                Message const & message_;
                body_visitor(Message const & message)
                    : message_(message) {}
                void operator()(typename value<typename Message::tag>::type const & body) const {
                    message_.body(body);
                }
                template <class T> void operator()(T const &) const {
                    // FIXME -- fail here
                }
            };

            template <class Tag, template <class> class Message>
            void operator()(Message<Tag> const & message) const {
                apply_visitor(body_visitor<Message<Tag> >(message), body_);
            }

        };

    } // namespace impl


    template <class Input>
    inline impl::body_directive const body(Input const & input) {
        return impl::body_directive(input);
    }

} // namespace network
} // namespace boost

#endif // __NETWORK_MESSAGE_DIRECTIVES_BODY_HPP__

