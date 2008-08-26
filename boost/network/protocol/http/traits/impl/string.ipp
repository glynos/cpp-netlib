
//          Copyright Dean Michael Berris 2008.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_NETWORK_PROTOCOL_HTTP_TRAITS_IMPL_STRING_IPP
#define BOOST_NETWORK_PROTOCOL_HTTP_TRAITS_IMPL_STRING_IPP

#include <boost/network/message/traits/string.hpp>
#include <boost/network/protocol/http/tags.hpp>

namespace boost { namespace network {

	template <>
		struct string<http::message_tag> {
			typedef http::string_traits<http::message_tag>::type type;
		};

} // namespace network

} // namespace boost

#endif // BOOST_NETWORK_PROTOCOL_HTTP_TRAITS_IMPL_STRING_IPP

