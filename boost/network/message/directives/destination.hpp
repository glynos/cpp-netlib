
//          Copyright Dean Michael Berris 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef __NETWORK_MESSAGE_DIRECTIVES_DESTINATION_HPP__
#define __NETWORK_MESSAGE_DIRECTIVES_DESTINATION_HPP__

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

    namespace impl {

        struct destination_directive {
            boost::variant<
                string<tags::default_string>::type,
                string<tags::default_wstring>::type,
                boost::shared_future<string<tags::default_string>::type>,
                boost::shared_future<string<tags::default_wstring>::type>
            > destination_;

            destination_directive(string<tags::default_string>::type const & destination)
                : destination_(destination) {}
            destination_directive(string<tags::default_wstring>::type const & destination)
                : destination_(destination) {}
            destination_directive(boost::shared_future<string<tags::default_string>::type> const & destination)
                : destination_(destination) {}
            destination_directive(boost::shared_future<string<tags::default_wstring>::type> const & destination)
                : destination_(destination) {}

            destination_directive(destination_directive const & other)
                : destination_(other.destination_) {}

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
            struct destination_visitor : boost::static_visitor<> {
                Message const & message_;
                destination_visitor(Message const & message)
                    : message_(message) {}
                void operator()(typename value<typename Message::tag>::type const & destination) const {
                    message_.destination(destination);
                }
                template <class T> void operator() (T const &) const {
                    // FIXME -- fail here!
                }
            };

            template <class Tag, template <class> class Message>
            void operator()(Message<Tag> const & message) const {
                apply_visitor(destination_visitor<Message<Tag> >(message), destination_);
            }
        };

    } // namespace impl

    template <class T>
    inline impl::destination_directive const
    destination(T const & destination_) {
        return impl::destination_directive(destination_);
    }

} // namespace network

} // namespace boost
 

#endif // __NETWORK_MESSAGE_DIRECTIVES_DESTINATION_HPP__
