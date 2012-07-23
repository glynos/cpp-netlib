//              Copyright 2012 Glyn Matthews.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#ifndef __BOOST_NETWORK_URI_SCHEMES_INC__
# define __BOOST_NETWORK_URI_SCHEMES_INC__

#include <string>
#include <boost/optional.hpp>

namespace network {
class hierarchical_schemes {

public:

    static bool exists(const std::string &scheme);

};

class opaque_schemes {

public:

    static bool exists(const std::string &scheme);

};

boost::optional<std::string> default_port(const std::string &scheme);
} // namespace network


#endif // __BOOST_NETWORK_URI_SCHEMES_INC__
