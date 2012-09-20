// Copyright Dean Michael Berris 2007.
// Copyright 2012 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef NETWORK_MESSAGE_TRANSFORMERS_HPP
#define NETWORK_MESSAGE_TRANSFORMERS_HPP

/** transformers.hpp
 *
 * Pulls in all the transformers files.
 */
#include <network/message/transformers/selectors.hpp>
#include <network/message/transformers/to_upper.hpp>
#include <network/message/transformers/to_lower.hpp>
#include <network/message/message_base.hpp>
#include <boost/type_traits.hpp>

namespace network {
namespace impl {

template <class Algorithm, class Selector>
struct get_real_algorithm {
    typedef typename boost::function_traits<
        typename boost::remove_pointer<
            Algorithm
        >::type
    >
    ::result_type::
    template type<
        typename boost::function_traits<
            typename boost::remove_pointer<
                Selector
            >::type
        >::result_type
    > type;
};

template <class Algorithm, class Selector>
struct transform_impl : public get_real_algorithm<Algorithm, Selector>::type {};

} // namspace impl

template <class Algorithm, class Selector>
inline impl::transform_impl<Algorithm, Selector>
transform(Algorithm, Selector) {
  return impl::transform_impl<Algorithm, Selector>();
}

template <class Algorithm, class Selector>
message_base & operator<< (
    message_base & msg_,
    impl::transform_impl<Algorithm, Selector> const & transformer) {
  transformer(msg_);
  return msg_;
}

} // namespace network

#endif // NETWORK_MESSAGE_TRANSFORMERS_HPP
