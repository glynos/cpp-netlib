
//          Copyright Dean Michael Berris 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef __NETWORK_MESSAGE_DIRECTIVES_DESTINATION_HPP__
#define __NETWORK_MESSAGE_DIRECTIVES_DESTINATION_HPP__

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
        template <class Tag>
            struct destination_directive : public detail::directive_base<Tag> {
                typedef Tag tag;

                explicit destination_directive ( std::string const & destination)
                    : _destination(destination)
                { };

                void operator() (basic_message<tag> & msg) const {
                    msg.destination() = _destination;
                };

                private:

                std::string _destination;
            };
    } // namespace impl

    inline impl::destination_directive<tags::default_>
        destination(std::string const & destination_) {
            return impl::destination_directive<tags::default_>(destination_);
        }

} // namespace network

} // namespace boost

#endif // __NETWORK_MESSAGE_DIRECTIVES_DESTINATION_HPP__
