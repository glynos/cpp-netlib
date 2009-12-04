
//          Copyright Dean Michael Berris 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef __NETWORK_MESSAGE_DIRECTIVES_BODY_HPP__
#define __NETWORK_MESSAGE_DIRECTIVES_BODY_HPP__

#include <boost/network/traits/string.hpp>


/** body.hpp
 *
 * Defines the types involved and the semantics of adding
 * body contents into message objects.
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
struct body_directive {

    explicit body_directive(T body) :
        _body(body)
    { };

    template <class MessageTag>
    void operator() (basic_message<MessageTag> & msg) const {
        msg.body() = _body;
    }

private:

    T _body;
};
} // namespace impl


// template <
//     class T
//     >
// inline
// impl::body_directive<T>
// body(T body_, boost::disable_if<T::message>::type) {
//     return impl::body_directive<T>(body_);
// }


inline
impl::body_directive<std::string>
body(std::string body_) {
    return impl::body_directive<std::string>(body_);
}


inline
impl::body_directive<std::wstring>
body(std::wstring body_) {
    return impl::body_directive<std::wstring>(body_);
}
} // namespace network
} // namespace boost

#endif // __NETWORK_MESSAGE_DIRECTIVES_BODY_HPP__

