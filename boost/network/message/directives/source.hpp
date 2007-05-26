
//          Copyright Dean Michael Berris 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef __NETWORK_MESSAGE_DIRECTIVES_SOURCE_HPP__
#define __NETWORK_MESSAGE_DIRECTIVES_SOURCE_HPP__

/** header.hpp
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
        template <class Tag>
            struct source_directive : public detail::directive_base<Tag> {
                typedef Tag tag;

                explicit source_directive ( std::string const & source)
                    : _source(source)
                { };

                void operator() (basic_message<tag> & msg) const {
                    msg.source() = _source;
                };
                
                private:

                mutable std::string _source;
            };
    } // namespace impl

    inline impl::source_directive<tags::default_>
        source(std::string const & source_) {
            return impl::source_directive<tags::default_>(source_);
        };

}; // namespace network

}; // namespace boost

#endif // __NETWORK_MESSAGE_DIRECTIVES_SOURCE_HPP__

