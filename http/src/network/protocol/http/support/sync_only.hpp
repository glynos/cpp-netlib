// Copyright Dean Michael Berris 2010.
// Copyright 2012 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef NETWORK_PROTOCOL_HTTP_SUPPORT_SYNC_ONLY_HPP_20100927
#define NETWORK_PROTOCOL_HTTP_SUPPORT_SYNC_ONLY_HPP_20100927

#include <network/protocol/http/tags.hpp>
#include <network/support/sync_only.hpp>

namespace network { namespace http {

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

}  // namespace http
    
}  // namespace network
    
#endif /* NETWORK_PROTOCOL_HTTP_SUPPORT_SYNC_ONLY_HPP_20100927 */
