#ifndef BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_RESOLVER_DELEGATE_FACTORY_HPP_20110930
#define BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_RESOLVER_DELEGATE_FACTORY_HPP_20110930

// Copyright 2011 Dean Michael Berris (dberris@google.com).
// Copyright 2011 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/shared_ptr.hpp>

namespace boost { namespace network { namespace http { namespace impl {

struct resolver_base;

template <class Tag, class Enable>
struct resolver_delegate_factory;

template <class Tag>
struct resolver_delegate_factory<Tag, typename enable_if<is_async<Tag> >::type> {
  typedef shared_ptr<resolver_base> resolver_delegate_ptr;
  // TODO Implement this!
  resolver_delegate_ptr new_resolver_delegate(...);
};

} /* impl */
} /* http */
} /* network */
} /* boost */

#endif /* BOOST_NETWORK_PROTOCOL_HTTP_CLIENT_CONNECTION_RESOLVER_DELEGATE_FACTORY_HPP_20110930 */
