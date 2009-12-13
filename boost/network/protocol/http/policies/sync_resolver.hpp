#ifndef BOOST_NETWORK_PROTOCOL_HTTP_POLICIES_SYNC_RESOLVER_20091214
#define BOOST_NETWORK_PROTOCOL_HTTP_POLICIES_SYNC_RESOLVER_20091214

//          Copyright Dean Michael Berris 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <utility>
#include <boost/asio.hpp>

namespace boost { namespace network { namespace http { namespace policies {

    template <class Tag>
    struct sync_resolver {

        typedef boost::asio::ip::tcp::resolver resolver_type;

        protected:

        typedef std::pair<
            boost::asio::ip::tcp::resolver::iterator,
            boost::asio::ip::tcp::resolver::iterator
        > resolver_iterator_pair;
        typedef typename string<Tag>::type string_type;
        typedef std::map<string_type, resolver_iterator_pair> resolved_cache;
        resolved_cache endpoint_cache_;
        bool cache_resolved_;

        sync_resolver(bool cache_resolved) : cache_resolved_(cache_resolved) {}

        resolver_iterator_pair resolve(resolver_type & resolver_, string_type const & hostname, string_type const & port) {
            if (cache_resolved_) {
                typename resolved_cache::iterator cached_iterator =
                    endpoint_cache_.find(hostname);
                if (cached_iterator == endpoint_cache_.end()) {
                    bool inserted = false;
                    boost::fusion::tie(cached_iterator, inserted) =
                        endpoint_cache_.insert(
                                std::make_pair(
                                        hostname,
                                        std::make_pair(
                                                resolver_.resolve(
                                                        boost::asio::ip::tcp::resolver::query(
                                                                hostname,
                                                                port,
                                                                boost::asio::ip::tcp::resolver_query::numeric_service
                                                                )
                                                        )
                                                        , boost::asio::ip::tcp::resolver::iterator()
                                                )
                                        )
                                );
                };
                return cached_iterator->second;
            };

            return std::make_pair(
                    resolver_.resolve(
                            boost::asio::ip::tcp::resolver::query(
                                    hostname,
                                    port,
                                    boost::asio::ip::tcp::resolver_query::numeric_service
                                    )
                            )
                            ,
                            boost::asio::ip::tcp::resolver::iterator()
                    );
        };

    };

} // namespace policies

} // namespace http

} // namespace network

} // namespace boost

#endif // BOOST_NETWORK_PROTOCOL_HTTP_POLICIES_SYNC_RESOLVER_20091214

