
//          Copyright Dean Michael Berris 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef __NETWORK_MESSAGE_DIRECTIVES_SOURCE_HPP__
#define __NETWORK_MESSAGE_DIRECTIVES_SOURCE_HPP__

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
        struct source_directive : public detail::directive_base<tags::default_> {

            explicit source_directive ( std::string const & source)
                : _source(source)
            { };

            template <class Tag>
            void operator() (basic_message<Tag> & msg) const {
                msg.source() = _source;
            }
            
            private:

            mutable std::string _source;
        };
    } // namespace impl

    inline impl::source_directive
        source(std::string const & source_) {
            return impl::source_directive(source_);
        }

} // namespace network

} // namespace boost

#endif // __NETWORK_MESSAGE_DIRECTIVES_SOURCE_HPP__

