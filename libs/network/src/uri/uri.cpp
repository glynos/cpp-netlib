//              Copyright 2012 Glyn Matthews.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#include <network/uri/uri.ipp>
#include <network/uri/uri.hpp>
#include <map>

#include <iterator>
#include <iostream>

namespace network {
bool operator == (const uri &lhs, const uri &rhs) {
	bool equal = boost::equal(
		std::make_pair(std::begin(lhs.scheme_range()), std::begin(lhs.path_range())),
		std::make_pair(std::begin(rhs.scheme_range()), std::begin(rhs.path_range())));
	if (equal)
	{
		// TODO: test normalized paths
		equal = boost::equal(lhs.path_range(), rhs.path_range());
	}

	if (equal)
	{
		// test query order
		std::map<uri::string_type, uri::string_type> lhs_query_params, rhs_query_params;
		equal = (query_map(lhs, lhs_query_params) ==	query_map(rhs, rhs_query_params));
	}

	return equal;
}
} // namespace network
