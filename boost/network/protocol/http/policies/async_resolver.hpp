#ifndef BOOST_NETWORK_PROTOCOL_HTTP_POLICIES_ASYNC_RESOLVER_20100622
#define BOOST_NETWORK_PROTOCOL_HTTP_POLICIES_ASYNC_RESOLVER_20100622

// Copyright Dean Michael Berris 2010.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

namespace boost { namespace network { namespace http { namespace policies {

    template <class Tag>
    struct async_resolver {
        typedef typename resolver<Tag>::type resolver_type;
        typedef typename resolver_type::iterator resolver_iterator;
        typedef typename resolver_type::query resolver_query;
        typedef std::pair<resolver_iterator, resolver_iterator> resolver_iterator_pair;

    protected:
        bool cache_resolved_;

        explicit async_resolver(bool cache_resolved)
            : cache_resolver_(cache_resolved)
        {}
    };

} // namespace policies

} // namespace http

} // namespace network

} // namespace boost

#endif // BOOST_NETWORK_PROTOCOL_HTTP_POLICIES_ASYNC_RESOLVER_20100622
