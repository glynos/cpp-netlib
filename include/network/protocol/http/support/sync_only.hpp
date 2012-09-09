#ifndef BOOST_NETWORK_PROTOCOL_HTTP_SUPPORT_SYNC_ONLY_HPP_20100927
#define BOOST_NETWORK_PROTOCOL_HTTP_SUPPORT_SYNC_ONLY_HPP_20100927

//          Copyright Dean Michael Berris 2010.
// Copyright 2012 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <network/protocol/http/tags.hpp>
#include <network/support/sync_only.hpp>

namespace boost { namespace network { namespace http {

    template <class Tag>
    struct sync_only :
        mpl::inherit_linearly<
            typename mpl::replace_if<
                typename tags::components<Tag>::type,
                is_same<mpl::placeholders::_,tags::async>,
                tags::sync
            >::type
            , mpl::inherit<mpl::placeholders::_1, mpl::placeholders::_2>
        >
    {};

} /* http */
    
} /* network */
    
} /* boost */

#endif /* BOOST_NETWORK_PROTOCOL_HTTP_SUPPORT_SYNC_ONLY_HPP_20100927 */
