//              Copyright 2012 Glyn Matthews.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#ifndef __BOOST_NETWORK_URI_SCHEMES_INC__
# define __BOOST_NETWORK_URI_SCHEMES_INC__


#include <boost/unordered_set.hpp>
#include <string>


namespace boost {
namespace network {
namespace uri {
class hierarchical_schemes {

public:

    static void register_(const std::string &scheme);
    static bool exists(const std::string &scheme);

private:

    static boost::unordered_set<std::string> schemes_;

};

class non_hierarchical_schemes {

public:

    static void register_(const std::string &scheme);
    static bool exists(const std::string &scheme);

private:

    static boost::unordered_set<std::string> schemes_;

};
} // namespace uri
} // namespace network
} // namespace boost


#endif // __BOOST_NETWORK_URI_SCHEMES_INC__
