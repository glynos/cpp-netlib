#ifndef BOOST_NETWORK_PROTOCOL_HTTP_SUPPORT_POD_OR_NORMAL_HPP_20101128
#define BOOST_NETWORK_PROTOCOL_HTTP_SUPPORT_POD_OR_NORMAL_HPP_20101128

// Copyright 2010 Dean Michael Berris.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/network/protocol/http/tags.hpp>
#include <boost/network/support/is_pod.hpp>
#include <boost/mpl/if.hpp>

namespace boost { namespace network {

    template <class Tag>
    struct pod_or_normal :
        mpl::if_<
            is_pod<Tag>
            , tags::pod
            , tags::normal
            >
    {};
    
} /* network */
    
} /* boost */

#endif /* BOOST_NETWORK_PROTOCOL_HTTP_SUPPORT_POD_OR_NORMAL_HPP_20101128 */
