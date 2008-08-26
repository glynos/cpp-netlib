
//          Copyright Dean Michael Berris 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef __NETWORK_MESSAGE_DIRECTIVES_HEADER_HPP__
#define __NETWORK_MESSAGE_DIRECTIVES_HEADER_HPP__

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
        template <class Tag>
            struct header_directive : public detail::directive_base<Tag> {
                typedef Tag tag;

                explicit header_directive(
                        std::string const & header_name, 
                        std::string const & header_value
                        ) :
                    _header_name(header_name),
                    _header_value(header_value)
                { };

                template <class MessageTag>
                void operator() (basic_message<MessageTag> & msg) const {
                    msg.headers().insert(
                            typename basic_message<MessageTag>::headers_container_type::value_type(
                                _header_name,
                                _header_value
                                )
                            );
                }

                private:

                mutable std::string _header_name;
                mutable std::string _header_value;
            };
    } // namespace impl

    template <class T1, class T2>
    inline impl::header_directive<tags::default_>
    header(T1 header_name,
            T2 header_value) {
        return impl::header_directive<tags::default_>(header_name,
                header_value);
    }

}

}

#endif // __NETWORK_MESSAGE_DIRECTIVES_HEADER_HPP__

