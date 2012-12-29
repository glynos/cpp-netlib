// Copyright 2010 (c) Dean Michael Berris
// Copyright 2012 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef NETWORK_SUPPORT_IS_HTTP_HPP_20100622
#define NETWORK_SUPPORT_IS_HTTP_HPP_20100622

#include <network/protocol/http/tags.hpp>
#include <boost/utility/enable_if.hpp>

namespace network { namespace http {

    template <class Tag, class Enable = void>
    struct is_http : mpl::false_ {};
    
    template <class Tag>
    struct is_http<Tag, typename enable_if<typename Tag::is_http>::type> : mpl::true_ {};
    
} // namespace http

} // namespace network

#endif // NETWORK_SUPPORT_IS_HTTP_HPP_20100622
