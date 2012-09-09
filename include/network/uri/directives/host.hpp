//            Copyright (c) Glyn Matthews 2011, 2012.
// Copyright 2012 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#ifndef __BOOST_NETWORK_URI_DIRECTIVES_HOST_INC__
# define __BOOST_NETWORK_URI_DIRECTIVES_HOST_INC__


# include <boost/range/begin.hpp>
# include <boost/range/end.hpp>


namespace network {
struct host_directive {

    explicit host_directive(const std::string &host)
        : host(host)
    {}

    template <
        class Uri
        >
    void operator () (Uri &uri) const {
        uri.append(host);
    }

    std::string host;

};

inline
host_directive host(const std::string &host)  {
    return host_directive(host);
}
} // namespace network


#endif // __BOOST_NETWORK_URI_DIRECTIVES_HOST_INC__
