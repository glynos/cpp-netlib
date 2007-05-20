
//          Copyright Dean Michael Berris 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef __NETWORK_MESSAGE_WRAPPERS_HEADERS_HPP__
#define __NETWORK_MESSAGE_WRAPPERS_HEADERS_HPP__

namespace boost { namespace network {

    // Forward declaration
    template <class Message>
        struct headers_range ;

    /** headers wrapper for messages.
     *
     * This exposes an interface similar to a map, indexable
     * using operator[] taking a string as the index and returns
     * a range of iterators (std::pair<iterator, iterator>)
     * whose keys are all equal to the index string.
     */
    namespace impl {
        template <class Tag>
            struct headers_wrapper : public detail::wrapper_base<Tag> {
                typedef Tag tag;
                typedef basic_message<tag> message_type;
                typedef typename headers_range<message_type>::type range_type;

                explicit headers_wrapper(basic_message<tag> & message_)
                    : detail::wrapper_base<tag>(message_)
                { };

                range_type operator[] (std::string const & key) const {
                    return headers_wrapper<Tag>::_message.headers().equal_range(key);
                };

                typename message_type::headers_container_type::size_type count(std::string const & key) const {
                    return headers_wrapper<Tag>::_message.headers().count(key);
                };

            };
    }; // namespace impl

    /// Factory method to create the right wrapper object
    template <class Tag, template <class> class Message>
        inline impl::headers_wrapper<Tag> 
        headers(Message<Tag> & message_) {
            return impl::headers_wrapper<Tag>(message_);
        };

    /// Template metaprogram to get the range type for a message
    template <class Message>
        struct headers_range {
            typedef typename 
                std::pair<
                    typename Message::headers_container_type::const_iterator,
                    typename Message::headers_container_type::const_iterator>
                type;
        };

}; // namespace network

}; // namespace boost

#endif // __NETWORK_MESSAGE_WRAPPERS_HEADERS_HPP__

