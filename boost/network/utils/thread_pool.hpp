#ifndef BOOST_NETWORK_UTILS_THREAD_POOL_HPP_20101020
#define BOOST_NETWORK_UTILS_THREAD_POOL_HPP_20101020

// Copyright 2010 Dean Michael Berris. 
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <cstddef>
#include <boost/network/tags.hpp>

namespace boost { namespace network { namespace utils {
    
    template <class Tag>
    struct basic_thread_pool {
        basic_thread_pool(std::size_t threads = 1) : threads_(threads) {}
        std::size_t const thread_count() const {
            return threads_;
        }
    protected:
        std::size_t threads_;
    };

    typedef basic_thread_pool<tags::default_> thread_pool;

} /* utils */
    
} /* network */
    
} /* boost */

#endif /* BOOST_NETWORK_UTILS_THREAD_POOL_HPP_20101020 */
