
//          Copyright Dean Michael Berris 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef __NETWORK_MESSAGE_DIRECTIVES_SOURCE_HPP__
#define __NETWORK_MESSAGE_DIRECTIVES_SOURCE_HPP__


#include <boost/network/traits/string.hpp>


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
    
namespace impl {
template <
    class T
    >
struct source_directive {

    explicit source_directive (T source)
        : _source(source)
    { };

    template <class MessageTag>
    void operator() (basic_message<MessageTag> & msg) const {
        msg.source() = _source;
    }
            
private:
    
    mutable T _source;
};
} // namespace impl

inline
impl::source_directive<std::string>
source(std::string source_) {
    return impl::source_directive<std::string>(source_);
}

inline
impl::source_directive<std::wstring>
source(std::wstring source_) {
    return impl::source_directive<std::wstring>(source_);
}
} // namespace network
} // namespace boost

#endif // __NETWORK_MESSAGE_DIRECTIVES_SOURCE_HPP__

