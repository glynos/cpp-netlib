//              Copyright 2012 Glyn Matthews.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#include <network/uri/uri.ipp>
#include <network/uri/uri.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <map>

#include <iterator>
#include <iostream>

namespace network {
bool operator == (const uri &lhs, const uri &rhs) {
	// the scheme can be compared insensitive to case
	bool equal = boost::iequals(lhs.scheme_range(), rhs.scheme_range());
	if (equal)
	{
		// the user info must be case sensitive
		equal = boost::equals(lhs.user_info_range(), rhs.user_info_range());
	}

	if (equal)
	{
		// the host can be compared insensitive to case
		equal = boost::iequals(
			std::make_pair(std::begin(lhs.host_range()), std::end(lhs.host_range())),
			std::make_pair(std::begin(rhs.host_range()), std::end(rhs.host_range())));
	}

	if (equal)
	{
		// TODO: test default ports according to scheme
		equal = boost::equals(
			std::make_pair(std::begin(lhs.port_range()), std::end(lhs.port_range())),
			std::make_pair(std::begin(rhs.port_range()), std::end(rhs.port_range())));
	}

	if (equal)
	{
		// TODO: test normalized paths
		equal = boost::iequals(lhs.path_range(), rhs.path_range());
	}

	if (equal)
	{
		// test query, independent of order
		std::map<uri::string_type, uri::string_type> lhs_query_params, rhs_query_params;
		equal = (query_map(lhs, lhs_query_params) == query_map(rhs, rhs_query_params));
	}

	return equal;
}
} // namespace network
