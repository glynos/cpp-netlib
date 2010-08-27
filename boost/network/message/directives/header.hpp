
//          Copyright Dean Michael Berris 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef __NETWORK_MESSAGE_DIRECTIVES_HEADER_HPP__
#define __NETWORK_MESSAGE_DIRECTIVES_HEADER_HPP__

#include <boost/network/traits/string.hpp>
#include <boost/network/support/is_async.hpp>
#include <boost/network/support/is_sync.hpp>
#include <boost/thread/future.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/or.hpp>

/** header.hpp
 *
 * Defines the types involved and the semantics of adding
 * header information into message objects.
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
        struct header_key :
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

        template <class Message>
        struct header_value :
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
   
template <class T1, class T2>
struct header_directive {

    explicit header_directive(T1 header_name, 
                              T2 header_value) :
        _header_name(header_name),
        _header_value(header_value)
    { };

    template <template <class> class Message>
    void operator() (Message const & msg) const {
        typedef typename Message::headers_container_type::value_type value_type;
        msg.add_header(value_type(_header_name, _header_value));
    }

private:

    T1 const & _header_name;
    T2 const & _header_value;
};
} // namespace impl

template <class T1, class T2>
inline impl::header_directive<T1, T2>
header(T1 const & header_name,
       T2 const & header_value) {
    return impl::header_directive<T1, T2>(header_name,
                                          header_value);
}
} // namespace network
} // namespace boost

#endif // __NETWORK_MESSAGE_DIRECTIVES_HEADER_HPP__

