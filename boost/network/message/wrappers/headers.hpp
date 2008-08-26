
//          Copyright Dean Michael Berris 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef __NETWORK_MESSAGE_WRAPPERS_HEADERS_HPP__
#define __NETWORK_MESSAGE_WRAPPERS_HEADERS_HPP__

#include <boost/network/message/traits/headers_container.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/range/functions.hpp>

namespace boost { namespace network {

	/// Template metaprogram to get the range type for a message
    template <class Message>
        struct headers_range {
			typedef typename headers_container<typename Message::tag>::type headers_container_type;
            typedef typename 
                boost::iterator_range<typename headers_container_type::const_iterator>
                type;
        };

    /** headers wrapper for messages.
     *
     * This exposes an interface similar to a map, indexable
     * using operator[] taking a string as the index and returns
     * a range of iterators (std::pair<iterator, iterator>)
     * whose keys are all equal to the index string.
	 *
	 * This type is also convertible to a 
	 *  headers_range<basic_message<tag> >::type
	 * Which allows for full range support.
	 *
     */
    namespace impl {
        template <class Tag>
            struct headers_wrapper : public detail::wrapper_base<Tag> {
                typedef Tag tag;
                typedef basic_message<Tag> message_type;
                typedef typename headers_range<message_type>::type range_type;
                typedef typename headers_container<Tag>::type headers_container_type;
				typedef typename headers_container_type::const_iterator const_iterator;
				typedef typename headers_container_type::iterator iterator;

                explicit headers_wrapper(basic_message<tag> & message_)
                    : detail::wrapper_base<tag>(message_)
                { };

                range_type operator[] (std::string const & key) const {
                    return headers_wrapper<Tag>::_message.headers().equal_range(key);
                };

                typename message_type::headers_container_type::size_type count(std::string const & key) const {
                    return headers_wrapper<Tag>::_message.headers().count(key);
                };

                const_iterator begin() const {
                    return headers_wrapper<Tag>::_message.headers().begin();
                };

                const_iterator end() const {
                    return headers_wrapper<Tag>::_message.headers().end();
                };

				operator range_type () {
					return make_iterator_range(headers_wrapper<Tag>::_message.headers().begin(), headers_wrapper<Tag>::_message.headers().end());
				};

            };
    } // namespace impl

    /// Factory method to create the right wrapper object
    template <class Tag, template <class> class Message>
        inline impl::headers_wrapper<Tag> 
        headers(Message<Tag> & message_) {
            return impl::headers_wrapper<Tag>(message_);
        }

} // namespace network

} // namespace boost

#endif // __NETWORK_MESSAGE_WRAPPERS_HEADERS_HPP__

