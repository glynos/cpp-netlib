
//          Copyright Dean Michael Berris 2008.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef NETWORK_MESSAGE_DIRECTIVES_REMOVE_HEADER_HPP
#define NETWORK_MESSAGE_DIRECTIVES_REMOVE_HEADER_HPP

namespace boost { namespace network {

	namespace impl {
		template <class Tag>
		struct remove_header_directive : public detail::directive_base<Tag> {
			typedef Tag tag;
			explicit remove_header_directive(
				std::string const & header_name
				) : 
			header_name_(header_name)
			{ };

			template <class MessageTag>
			void operator() (basic_message<MessageTag> & msg) const {
				msg.headers().erase(header_name_);
			}

		private:
			mutable std::string header_name_;
		};

	} // namespace impl

	template <class T1>
	inline impl::remove_header_directive<tags::default_>
		remove_header(T1 header_name) {
			return impl::remove_header_directive<tags::default_>(header_name);
	}

} // namespace network

} // namespace boost

#endif // NETWORK_MESSAGE_DIRECTIVES_REMOVE_HEADER_HPP

