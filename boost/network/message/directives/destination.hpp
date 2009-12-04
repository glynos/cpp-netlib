
//          Copyright Dean Michael Berris 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef __NETWORK_MESSAGE_DIRECTIVES_DESTINATION_HPP__
#define __NETWORK_MESSAGE_DIRECTIVES_DESTINATION_HPP__

#include <boost/network/traits/string.hpp>


/** destination.hpp
 *
 * Defines the types involved and the semantics of adding
 * destination information into message objects.
 *
 * WARNING: DO NOT INCLUDE THIS HEADER DIRECTLY. THIS REQUIRES
 * TYPES TO BE DEFINED FROM EARLIER FILES THAT INCLUDE THIS
 * HEADER.
 */
namespace boost { namespace network {
    
namespace impl {
template <class T>
struct destination_directive {

    explicit destination_directive (T destination)
        : _destination(destination)
    { };

    template <
        class MessageTag
        >
    void operator() (basic_message<MessageTag> & msg) const {
        msg.destination() = _destination;
    };

private:
    
    T _destination;
};
} // namespace impl


inline
impl::destination_directive<std::string>
destination(std::string destination_) {
    return impl::destination_directive<std::string>(destination_);
}

inline
impl::destination_directive<std::wstring>
destination(std::wstring destination_) {
    return impl::destination_directive<std::wstring>(destination_);
}
} // namespace network
} // namespace boost


#endif // __NETWORK_MESSAGE_DIRECTIVES_DESTINATION_HPP__
