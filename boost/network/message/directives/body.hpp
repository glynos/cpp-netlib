
//          Copyright Dean Michael Berris 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef __NETWORK_MESSAGE_DIRECTIVES_BODY_HPP__
#define __NETWORK_MESSAGE_DIRECTIVES_BODY_HPP__

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
        template <class Tag>
            struct body_directive : public detail::directive_base<Tag> {
                typedef Tag tag;

                explicit body_directive(
                        std::string const & body
                        ) :
                    _body(body)
                { };

                template <class MessageTag>
                void operator() (basic_message<MessageTag> & msg) const {
                    msg.body() = _body;
                }

                private:

                std::string _body;
            };
    } // namespace impl

    inline impl::body_directive<tags::default_>
        body(std::string const & body_) {
            return impl::body_directive<tags::default_>(body_);
        }

} // namespace network

} // namespace boost

#endif // __NETWORK_MESSAGE_DIRECTIVES_BODY_HPP__

