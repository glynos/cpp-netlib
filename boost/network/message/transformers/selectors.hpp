
//          Copyright Dean Michael Berris 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef __NETWORK_MESSAGE_TRANSFORMERS_SELECTORS_HPP__
#define __NETWORK_MESSAGE_TRANSFORMERS_SELECTORS_HPP__

namespace boost { namespace network {
    namespace selectors {
        struct source_selector { };
        struct destination_selector { };
    }; // namespace selectors

    extern selectors::source_selector source_;
    extern selectors::destination_selector destination_;

}; // namespace network

}; // namespace boost

#endif // __NETWORK_MESSAGE_TRANSFORMERS_SELECTORS_HPP__

