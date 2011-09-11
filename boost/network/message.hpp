//          Copyright Dean Michael Berris 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef __NETWORK_MESSAGE_HPP__
#define __NETWORK_MESSAGE_HPP__

#include <boost/network/message_fwd.hpp>
#include <boost/network/detail/directive_base.hpp>
#include <boost/network/detail/wrapper_base.hpp>
#include <boost/network/message/directives.hpp>
#include <boost/network/message/wrappers.hpp>
#include <boost/network/message/transformers.hpp>

#include <boost/network/message/modifiers/add_header.hpp>
#include <boost/network/message/modifiers/remove_header.hpp>
#include <boost/network/message/modifiers/clear_headers.hpp>
#include <boost/network/message/modifiers/source.hpp>
#include <boost/network/message/modifiers/destination.hpp>
#include <boost/network/message/modifiers/body.hpp>

#ifdef BOOST_NETWORK_DEBUG
#include <boost/network/message/message_concept.hpp>
#endif

#include <boost/network/message/basic_message.hpp>

/** message.hpp
 *
 * This header file implements the common message type which
 * all networking implementations under the boost::network
 * namespace. The common message type allows for easy message
 * construction and manipulation suited for networked
 * application development.
 */
namespace boost { namespace network {

    /** The common message type.
     */
    template <class String>
    struct basic_message : basic_storage_base {
        typedef std::pair<String, String> header_type;
        typedef std::multimap<String, String> headers_container_type;
        typedef String string_type;

        basic_message()
        : basic_storage_base()
        {}

        basic_message(const basic_message & other)
        : basic_storage_base(other)
        {}

        basic_message & operator=(basic_message<String> rhs) {
            rhs.swap(*this);
            return *this;
        }

        void swap(basic_message<String> & other) {
          basic_storage_base & this_ = *this,
                             & other_ = other;
          swap(this_, other_);
        }

        headers_container_type & headers() {
          return pimpl->headers_;
        }

        void headers(headers_container_type const & headers_) const {
          pimpl->headers_ = headers_;
        }

        void add_header(typename headers_container_type::value_type const & pair_) const {
          this->append_header(pair_.first, pair_.second);
        }

        void remove_header(typename headers_container_type::key_type const & key) const {
          this->remove_headers(key);
        }

        headers_container_type const & headers() const {
          return pimpl->headers_;
        }

        string_type & body() {
          return pimpl->body_;
        }

        void body(string_type const & body_) const {
          this->set_body(body_);
        }

        string_type const & body() const {
          return pimpl->body_;
        }
        
        string_type & source() {
          return pimpl->source_;
        }

        void source(string_type const & source_) const {
          this->set_source(source_);
        }

        string_type const & source() const {
          return pimpl->source_;
        }

        string_type & destination() {
          return pimpl->destination_;
        }

        void destination(string_type const & destination_) const {
          this->set_destination(destination_);
        }

        string_type const & destination() const {
          return pimpl->destination_;
        }
    };

    template <class String>
    inline void swap(basic_message<String> & left, basic_message<String> & right) {
        left.swap(right);
    }
    
    // Commenting this out as we don't need to do this anymore.
    // BOOST_CONCEPT_ASSERT((Message<basic_message<boost::network::tags::default_string> >));
    // BOOST_CONCEPT_ASSERT((Message<basic_message<boost::network::tags::default_wstring> >));
    typedef basic_message<std::string> message;
    typedef basic_message<std::wstring> wmessage;

} // namespace network
} // namespace boost

#endif // __NETWORK_MESSAGE_HPP__

