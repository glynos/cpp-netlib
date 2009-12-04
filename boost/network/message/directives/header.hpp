
//          Copyright Dean Michael Berris 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef __NETWORK_MESSAGE_DIRECTIVES_HEADER_HPP__
#define __NETWORK_MESSAGE_DIRECTIVES_HEADER_HPP__


#include <boost/network/traits/string.hpp>

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

namespace impl {
   
template <class T1, class T2>
struct header_directive {

    explicit header_directive(T1 header_name, 
                              T2 header_value) :
        _header_name(header_name),
        _header_value(header_value)
    { };

    template <class MessageTag>
    void operator() (basic_message<MessageTag> & msg) const {
        typedef typename basic_message<MessageTag>::headers_container_type::value_type value_type;
        msg.headers().insert(value_type(_header_name, _header_value));
    }

private:

    mutable T1 _header_name;
    mutable T2 _header_value;
};
} // namespace impl

template <class T1, class T2>
inline impl::header_directive<T1, T2>
header(T1 header_name,
       T2 header_value) {
    return impl::header_directive<T1, T2>(header_name,
                                          header_value);
}
} // namespace network
} // namespace boost

#endif // __NETWORK_MESSAGE_DIRECTIVES_HEADER_HPP__

