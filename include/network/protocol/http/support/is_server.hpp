// Copyright 2010 Dean Michael Berris. 
// Copyright 2012 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef NETWORK_PROTOCOL_SUPPORT_IS_SERVER_HPP_20101118
#define NETWORK_PROTOCOL_SUPPORT_IS_SERVER_HPP_20101118

#include <boost/utility/enable_if.hpp>

namespace network { namespace http {

    template <class Tag, class Enable = void>
    struct is_server : mpl::false_ {};
    
    template <class Tag>
    struct is_server<Tag, typename enable_if<typename Tag::is_server>::type> : mpl::true_ {};
    
}  // namespace http
    
}  // namespace network
    
#endif /* NETWORK_PROTOCOL_SUPPORT_IS_SERVER_HPP_20101118 */
